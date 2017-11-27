/**
 * php-asio/socket.cpp
 *
 * @author CismonX<admin@cismon.net>
 */

#include "socket.hpp"

namespace Asio
{
    template <typename Protocol>
    Php::Value Socket<Protocol>::connect_handler(const boost::system::error_code& error)
    {
        if (callback_.isCallable())
            Future::coroutine(callback_(this, error.value(), argument_));
        return Php::Value();
    }

    template <typename Protocol> template <typename, typename>
    Php::Value Socket<Protocol>::read_handler(const boost::system::error_code& error, size_t length, std::vector<uint8_t>* buffer)
    {
        Php::Value str_buffer(std::string(buffer->begin(), buffer->end()));
        if (callback_.isCallable())
            Future::coroutine(callback_(this, str_buffer, static_cast<int64_t>(length), error.value(), argument_));
        delete buffer;
        return str_buffer;
    }

    template <typename Protocol> template <typename, typename>
    Php::Value Socket<Protocol>::write_handler(const boost::system::error_code& error, size_t length, std::string* buffer)
    {
        if (callback_.isCallable())
            Future::coroutine(callback_(this, static_cast<int64_t>(length), error.value(), argument_));
        delete buffer;
        return static_cast<int64_t>(length);
    }

    template <typename Protocol> template <typename, typename>
    Future* Socket<Protocol>::read(int64_t length, bool read_some)
    {
        if (cancelled_)
            throw Php::Exception("Trying to read on a closed socket.");
        auto size = static_cast<size_t>(length);
        auto buffer_container = new std::vector<uint8_t>(size);
        auto buffer = boost::asio::buffer(*buffer_container, size);
        auto future = Future::add();
        future->on_resolve<size_t>(boost::bind(&Socket::read_handler, this, _1, _2, buffer_container));
        if (read_some)
            socket_.async_read_some(buffer, ASYNC_HANDLER_DOUBLE_ARG(size_t));
        else
            async_read(socket_, buffer, ASYNC_HANDLER_DOUBLE_ARG(size_t));
        return future;
    }

    template <typename Protocol> template <typename, typename>
    Future* Socket<Protocol>::write(const std::string& data, bool write_some)
    {
        if (cancelled_)
            throw Php::Exception("Trying to write to a closed socket.");
        // Note that `data` gets out of scope when handler callback is called.
        auto buffer = new std::string(data);
        auto future = Future::add();
        future->on_resolve<size_t>(boost::bind(&Socket::write_handler, this, _1, _2, buffer));
        if (write_some)
            socket_.async_write_some(boost::asio::buffer(*buffer), ASYNC_HANDLER_DOUBLE_ARG(size_t));
        else
            async_write(socket_, boost::asio::buffer(*buffer), ASYNC_HANDLER_DOUBLE_ARG(size_t));
        return future;
    }

    template <>
    TcpSocket::Socket(boost::asio::io_service& io_service) : Base(io_service), socket_(io_service)
    {
        wrapper_ = new Php::Object("Asio\\TcpSocket", this);
    }

    template <>
    UnixSocket::Socket(boost::asio::io_service& io_service) : Base(io_service), socket_(io_service)
    {
        wrapper_ = new Php::Object("Asio\\UnixSocket", this);
    }

    template <typename Protocol>
    typename Protocol::socket& Socket<Protocol>::getSocket()
    {
        return socket_;
    }

    template <typename Protocol> template <typename P, std::string(P::endpoint::*port)()>
    Php::Value Socket<Protocol>::open_inet(Php::Parameters& params)
    {
        boost::system::error_code ec;
        socket_.open(params[0].boolValue() ? Protocol::v6() : Protocol::v4(), ec);
        return ec.value();
    }

    template <typename Protocol> template <typename P, std::string(P::endpoint::*path)()>
    Php::Value Socket<Protocol>::open_local()
    {
        boost::system::error_code ec;
        socket_.open(Protocol(), ec);
        return ec.value();
    }

    template <>
    Php::Value TcpSocket::bind(Php::Parameters& params)
    {
        boost::system::error_code ec;
        socket_.bind({ boost::asio::ip::address::from_string(params[0].stringValue()),
            static_cast<unsigned short>(params[1].numericValue()) }, ec);
        return ec.value();
    }

    template <>
    Php::Value UnixSocket::bind(Php::Parameters& params)
    {
        auto path = params[0].stringValue();
        // Unlink socket file before binding.
        if (boost::filesystem::status(path).type() == boost::filesystem::socket_file)
            boost::filesystem::remove(path);
        boost::system::error_code ec;
        socket_.bind({ path }, ec);
        return ec.value();
    }

    template <> template <>
    Future* TcpSocket::connect(const std::string& address, unsigned short port_num)
    {
        auto future = Future::add();
        future->on_resolve<NOARG>(boost::bind(&Socket::connect_handler, this, _1));
        socket_.async_connect({ boost::asio::ip::address::from_string(address), port_num }, ASYNC_HANDLER_SINGLE_ARG);
        return future;
    }

    template <> template <>
    Future* UnixSocket::connect(const std::string& socket_path)
    {
        auto future = Future::add();
        future->on_resolve<NOARG>(boost::bind(&Socket::connect_handler, this, _1));
        socket_.async_connect({ socket_path }, ASYNC_HANDLER_SINGLE_ARG);
        return future;
    }

    template <>
    Php::Value TcpSocket::connect(Php::Parameters& params)
    {
        auto param_count = params.size();
        callback_ = param_count > 2 ? params[2] : Php::Value();
        argument_ = param_count > 3 ? params[3] : Php::Value();
        return connect(params[0].stringValue(), static_cast<unsigned short>(params[1].numericValue()));
    }

    template <>
    Php::Value UnixSocket::connect(Php::Parameters& params)
    {
        auto param_count = params.size();
        callback_ = param_count > 1 ? params[1] : Php::Value();
        argument_ = param_count > 2 ? params[2] : Php::Value();
        return connect(params[0].stringValue());
    }

    template <typename Protocol> template <typename, typename>
    Php::Value Socket<Protocol>::read(Php::Parameters& params)
    {
        auto length = params[0].numericValue();
        if (length < 0)
            throw Php::Exception("Non-negative integer expected.");
        auto param_count = params.size();
        auto read_some = param_count < 2 ? true : params[1].boolValue();
        callback_ = param_count < 3 ? Php::Value() : params[2];
        argument_ = param_count < 4 ? Php::Value() : params[3];
        return read(length, read_some);
    }

    template <typename Protocol> template <typename, typename>
    Php::Value Socket<Protocol>::write(Php::Parameters& params)
    {
        auto param_count = params.size();
        auto write_some = param_count < 2 ? false : params[1].boolValue();
        callback_ = param_count < 3 ? Php::Value() : params[2];
        argument_ = param_count < 4 ? Php::Value() : params[3];
        return write(params[0].stringValue(), write_some);
    }

    template <> template <>
    Php::Value TcpSocket::remote_addr() const
    {
        return socket_.remote_endpoint().address().to_string();
    }

    template <> template <>
    Php::Value TcpSocket::remote_port() const
    {
        return static_cast<int64_t>(socket_.remote_endpoint().port());
    }

    template <> template <>
    Php::Value UnixSocket::remote_path() const
    {
        return socket_.remote_endpoint().path();
    }

    template <typename Protocol>
    Php::Value Socket<Protocol>::available(Php::Parameters& params) const
    {
        boost::system::error_code ec;
        auto bytes = socket_.available(ec);
        if (params.size())
            params[0] = ec.value();
        return static_cast<int64_t>(bytes);
    }

    template <typename Protocol>
    Php::Value Socket<Protocol>::at_mark(Php::Parameters& params) const
    {
        boost::system::error_code ec;
        auto at_mark = socket_.at_mark(ec);
        if (params.size())
            params[0] = ec.value();
        return static_cast<int64_t>(at_mark);
    }

    template <typename Protocol>
    Php::Value Socket<Protocol>::close()
    {
        if (cancelled_)
            throw Php::Exception("Trying to closed a closed socket.");
        boost::system::error_code ec;
        socket_.cancel(ec);
        if (!ec) {
            socket_.close(ec);
            if (!ec) {
                cancelled_ = true;
                delete wrapper_;
            }
        }
        return ec.value();
    }

    // Instantiation for TcpSocket.
    template class Socket<tcp>;
    template Php::Value TcpSocket::open_inet(Php::Parameters&);
    template Php::Value TcpSocket::read(Php::Parameters&);
    template Php::Value TcpSocket::read_handler(const boost::system::error_code&, size_t, std::vector<uint8_t>*);
    template Php::Value TcpSocket::write(Php::Parameters&);
    template Php::Value TcpSocket::write_handler(const boost::system::error_code&, size_t, std::string*);
    template Future* TcpSocket::read(int64_t, bool);
    template Future* TcpSocket::write(const std::string&, bool);

    // Instantiation for UnixSocket.
    template class Socket<unix>;
    template Php::Value UnixSocket::open_local();
    template Php::Value UnixSocket::read(Php::Parameters&);
    template Php::Value UnixSocket::read_handler(const boost::system::error_code&, size_t, std::vector<uint8_t>*);
    template Php::Value UnixSocket::write(Php::Parameters&);
    template Php::Value UnixSocket::write_handler(const boost::system::error_code&, size_t, std::string*);
    template Future* UnixSocket::read(int64_t, bool);
    template Future* UnixSocket::write(const std::string&, bool);
}
