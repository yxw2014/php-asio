/**
 * php-asio/socket.cpp
 *
 * @author CismonX<admin@cismon.net>
 */

#include "socket.hpp"

namespace Asio
{
    template <typename Protocol>
    CORO_RETVAL Socket<Protocol>::connect_handler(
        const boost::system::error_code& error,
        const Php::Value& callback,
        const Php::Value& argument)
    {
        if (callback.isCallable())
            CORO_REGISTER(callback(this, error.value(), argument));
        CORO_RETURN(Php::Value());
    }

    template <typename Protocol> template <typename, typename>
    CORO_RETVAL Socket<Protocol>::read_handler(
        const boost::system::error_code& error,
        size_t length,
        std::vector<uint8_t>* buffer,
        const Php::Value& callback,
        const Php::Value& argument)
    {
        Php::Value str_buffer(std::string(buffer->begin(), buffer->end()));
        Php::Value bytes_transferred = static_cast<int64_t>(length);
        if (callback.isCallable())
            CORO_REGISTER(callback(this, str_buffer, bytes_transferred, error.value(), argument));
        delete buffer;
#if ENABLE_COROUTINE
        return std::map<std::string, Php::Value> {
            std::make_pair("data", str_buffer),
            std::make_pair("length", bytes_transferred)
        };
#endif
    }

    template <typename Protocol>
    CORO_RETVAL Socket<Protocol>::write_handler(
        const boost::system::error_code& error,
        size_t length,
        std::string* buffer,
        const Php::Value& callback,
        const Php::Value& argument)
    {
        if (callback.isCallable())
            CORO_REGISTER(callback(this, static_cast<int64_t>(length), error.value(), argument));
        delete buffer;
        CORO_RETURN(static_cast<int64_t>(length));
    }

    template <>
    CORO_RETVAL UdpSocket::recv_handler(
        const boost::system::error_code& error,
        size_t length,
        std::vector<uint8_t>* buffer,
        udp::endpoint* endpoint,
        const Php::Value& callback,
        const Php::Value& argument)
    {
        Php::Value str_buffer(std::string(buffer->begin(), buffer->end()));
        Php::Value address = endpoint->address().to_string();
        Php::Value bytes_transferred = static_cast<int64_t>(length);
        Php::Value port = endpoint->port();
        if (callback.isCallable())
            CORO_REGISTER(callback(this, str_buffer, address, port, bytes_transferred, error.value(), argument));
        delete buffer;
        delete endpoint;
#if ENABLE_COROUTINE
        return std::map<std::string, Php::Value> {
            std::make_pair("data", str_buffer),
            std::make_pair("length", bytes_transferred),
            std::make_pair("address", address),
            std::make_pair("port", port)
        };
#endif
    }

    template <>
    CORO_RETVAL UdgSocket::recv_handler(
        const boost::system::error_code& error,
        size_t length,
        std::vector<uint8_t>* buffer,
        udg::endpoint* endpoint,
        const Php::Value& callback,
        const Php::Value& argument)
    {
        Php::Value str_buffer(std::string(buffer->begin(), buffer->end()));
        Php::Value bytes_transferred = static_cast<int64_t>(length);
        auto path = endpoint->path();
        if (callback.isCallable())
            CORO_REGISTER(callback(this, str_buffer, path, bytes_transferred, error.value(), argument));
        delete buffer;
        delete endpoint;
#if ENABLE_COROUTINE
        return std::map<std::string, Php::Value> {
            std::make_pair("data", str_buffer),
            std::make_pair("length", bytes_transferred),
            std::make_pair("path", path),
        };
#endif
    }

    template <> template <>
    Future* TcpSocket::connect(
        const std::string& address,
        unsigned short port_num,
        const Php::Value& callback,
        const Php::Value& argument)
    {
        auto future = Future::add();
        future->on_resolve<NOARG>(boost::bind(&Socket::connect_handler, this, _1, callback, argument));
        socket_.async_connect({ boost::asio::ip::address::from_string(address), port_num }, ASYNC_HANDLER_SINGLE_ARG);
        return future;
    }

    template <> template <>
    Future* UnixSocket::connect(
        const std::string& socket_path,
        const Php::Value& callback,
        const Php::Value& argument)
    {
        auto future = Future::add();
        future->on_resolve<NOARG>(boost::bind(&Socket::connect_handler, this, _1, callback, argument));
        socket_.async_connect({ socket_path }, ASYNC_HANDLER_SINGLE_ARG);
        return future;
    }

    template <typename Protocol> template <typename, typename>
    Future* Socket<Protocol>::read(int64_t length, bool read_some, const Php::Value& callback, const Php::Value& argument)
    {
        if (cancelled_)
            throw Php::Exception("Trying to read on a closed socket.");
        auto size = static_cast<size_t>(length);
        auto buffer_container = new std::vector<uint8_t>(size);
        auto buffer = boost::asio::buffer(*buffer_container, size);
        auto future = Future::add();
        future->on_resolve<size_t>(boost::bind(
            &Socket::read_handler, this, _1, _2, buffer_container, callback, argument));
        if (read_some)
            socket_.async_read_some(buffer, ASYNC_HANDLER_DOUBLE_ARG(size_t));
        else
            async_read(socket_, buffer, ASYNC_HANDLER_DOUBLE_ARG(size_t));
        return future;
    }

    template <typename Protocol> template <typename, typename>
    Future* Socket<Protocol>::write(
        const std::string& data,
        bool write_some,
        const Php::Value& callback,
        const Php::Value& argument)
    {
        if (cancelled_)
            throw Php::Exception("Trying to write to a closed socket.");
        // Note that `data` gets out of scope when handler callback is called.
        auto buffer = new std::string(data);
        auto future = Future::add();
        future->on_resolve<size_t>(boost::bind(&Socket::write_handler, this, _1, _2, buffer, callback, argument));
        if (write_some)
            socket_.async_write_some(boost::asio::buffer(*buffer), ASYNC_HANDLER_DOUBLE_ARG(size_t));
        else
            async_write(socket_, boost::asio::buffer(*buffer), ASYNC_HANDLER_DOUBLE_ARG(size_t));
        return future;
    }

    template <typename Protocol> template <typename P, typename>
    Future* Socket<Protocol>::recv_from(int64_t length, const Php::Value& callback, const Php::Value& argument)
    {
        if (cancelled_)
            throw Php::Exception("Trying to receive from a closed socket.");
        auto size = static_cast<size_t>(length);
        auto buffer_container = new std::vector<uint8_t>(size);
        auto buffer = boost::asio::buffer(*buffer_container, size);
        auto endpoint = new typename P::endpoint;
        auto future = Future::add();
        future->on_resolve<size_t>(boost::bind(
            &Socket::recv_handler, this, _1, _2, buffer_container, endpoint, callback, argument));
        socket_.async_receive_from(buffer, *endpoint, ASYNC_HANDLER_DOUBLE_ARG(size_t));
        return future;
    }

    template <typename Protocol> template <typename P, typename>
    Future* Socket<Protocol>::send_to(
        const std::string& data,
        const typename P::endpoint& endpoint,
        const Php::Value& callback,
        const Php::Value& argument)
    {
        if (cancelled_)
            throw Php::Exception("Trying to send to a closed socket.");
        auto buffer = new std::string(data);
        auto future = Future::add();
        future->on_resolve<size_t>(boost::bind(
            &Socket::write_handler, this, _1, _2, buffer, callback, argument));
        socket_.async_send_to(boost::asio::buffer(*buffer), endpoint, ASYNC_HANDLER_DOUBLE_ARG(size_t));
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

    template <>
    UdpSocket::Socket(boost::asio::io_service& io_service) : Base(io_service), socket_(io_service)
    {
        wrapper_ = new Php::Object("Asio\\UdpSocket", this);
    }

    template <>
    UdgSocket::Socket(boost::asio::io_service& io_service) : Base(io_service), socket_(io_service)
    {
        wrapper_ = new Php::Object("Asio\\UdgSocket", this);
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

    template <typename Protocol> template <typename P, std::string(P::endpoint::*port)()>
    Php::Value Socket<Protocol>::bind_inet(Php::Parameters& params)
    {
        boost::system::error_code ec;
        socket_.bind({ boost::asio::ip::address::from_string(params[0].stringValue()),
            static_cast<unsigned short>(params[1].numericValue()) }, ec);
        return ec.value();
    }

    template <typename Protocol> template <typename P, std::string(P::endpoint::*path)()>
    Php::Value Socket<Protocol>::bind_local(Php::Parameters& params)
    {
        auto socket_path = params[0].stringValue();
        // Unlink socket file before binding.
        if (boost::filesystem::status(socket_path).type() == boost::filesystem::socket_file)
            boost::filesystem::remove(socket_path);
        boost::system::error_code ec;
        socket_.bind({ socket_path }, ec);
        return ec.value();
    }

    template <>
    FUTURE_RETVAL TcpSocket::connect(Php::Parameters& params)
    {
        auto param_count = params.size();
        FUTURE_RETURN connect(params[0].stringValue(), static_cast<unsigned short>(params[1].numericValue()),
            param_count > 2 ? params[2] : Php::Value(), param_count > 3 ? params[3] : Php::Value());
    }

    template <>
    FUTURE_RETVAL UnixSocket::connect(Php::Parameters& params)
    {
        auto param_count = params.size();
        FUTURE_RETURN connect(params[0].stringValue(),
            param_count > 2 ? params[2] : Php::Value(), param_count > 3 ? params[3] : Php::Value());
    }

    template <typename Protocol> template <typename, typename>
    FUTURE_RETVAL Socket<Protocol>::read(Php::Parameters& params)
    {
        auto length = params[0].numericValue();
        if (length < 0)
            throw Php::Exception("Non-negative integer expected.");
        auto param_count = params.size();
        auto read_some = param_count < 2 ? true : params[1].boolValue();
        FUTURE_RETURN read(length, read_some,
            param_count < 3 ? Php::Value() : params[2], param_count < 4 ? Php::Value() : params[3]);
    }

    template <typename Protocol> template <typename, typename>
    FUTURE_RETVAL Socket<Protocol>::write(Php::Parameters& params)
    {
        auto param_count = params.size();
        auto write_some = param_count < 2 ? false : params[1].boolValue();
        FUTURE_RETURN write(params[0].stringValue(), write_some,
            param_count < 3 ? Php::Value() : params[2], param_count < 4 ? Php::Value() : params[3]);
    }

    template <typename Protocol> template <typename, typename>
    FUTURE_RETVAL Socket<Protocol>::recv_from(Php::Parameters& params)
    {
        auto length = params[0].numericValue();
        if (length < 0)
            throw Php::Exception("Non-negative integer expected.");
        auto param_count = params.size();
        FUTURE_RETURN recv_from(length,
            param_count < 2 ? Php::Value() : params[1], param_count < 3 ? Php::Value() : params[2]);
    }

    template <> template <>
    FUTURE_RETVAL UdpSocket::send_to(Php::Parameters& params)
    {
        auto param_count = params.size();
        udp::endpoint endpoint(boost::asio::ip::address::from_string(params[1].stringValue()),
            static_cast<unsigned short>(params[2].numericValue()));
        FUTURE_RETURN send_to(params[0].stringValue(), endpoint,
            param_count < 4 ? Php::Value() : params[3], param_count < 5 ? Php::Value() : params[4]);
    }

    template <> template <>
    FUTURE_RETVAL UdgSocket::send_to(Php::Parameters& params)
    {
        auto param_count = params.size();
        FUTURE_RETURN send_to(params[0].stringValue(), { params[1].stringValue() },
            param_count < 3 ? Php::Value() : params[2], param_count < 4 ? Php::Value() : params[3]);
    }

    template <typename Protocol> template <typename P, std::string(P::endpoint::*port)()>
    Php::Value Socket<Protocol>::remote_addr() const
    {
        return socket_.remote_endpoint().address().to_string();
    }

    template <typename Protocol> template <typename P, std::string(P::endpoint::*port)()>
    Php::Value Socket<Protocol>::remote_port() const
    {
        return static_cast<int64_t>(socket_.remote_endpoint().port());
    }

    template <typename Protocol> template <typename P, std::string(P::endpoint::*path)()>
    Php::Value Socket<Protocol>::remote_path() const
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

    template class Socket<tcp>;
    template Php::Value TcpSocket::open_inet(Php::Parameters&);
    template Php::Value TcpSocket::bind_inet(Php::Parameters&);
    template Future* TcpSocket::read(int64_t, bool, const Php::Value&, const Php::Value&);
    template Future* TcpSocket::write(const std::string&, bool, const Php::Value&, const Php::Value&);
    template CORO_RETVAL TcpSocket::read_handler(const boost::system::error_code&,
        size_t, std::vector<uint8_t>*, const Php::Value&, const Php::Value&);
    template FUTURE_RETVAL TcpSocket::read(Php::Parameters&);
    template FUTURE_RETVAL TcpSocket::write(Php::Parameters&);
    template Php::Value TcpSocket::remote_addr() const;
    template Php::Value TcpSocket::remote_port() const;

    template class Socket<unix>;
    template Php::Value UnixSocket::open_local();
    template Php::Value UnixSocket::bind_local(Php::Parameters&);
    template Future* UnixSocket::read(int64_t, bool, const Php::Value&, const Php::Value&);
    template Future* UnixSocket::write(const std::string&, bool, const Php::Value&, const Php::Value&);
    template CORO_RETVAL UnixSocket::read_handler(const boost::system::error_code&,
        size_t, std::vector<uint8_t>*, const Php::Value&, const Php::Value&);
    template FUTURE_RETVAL UnixSocket::read(Php::Parameters&);
    template FUTURE_RETVAL UnixSocket::write(Php::Parameters&);
    template Php::Value UnixSocket::remote_path() const;

    template class Socket<udp>;
    template Php::Value UdpSocket::open_inet(Php::Parameters&);
    template Php::Value UdpSocket::bind_inet(Php::Parameters&);
    template Future* UdpSocket::recv_from(int64_t, const Php::Value&, const Php::Value&);
    template Future* UdpSocket::send_to(const std::string&, const udp::endpoint&, const Php::Value&, const Php::Value&);
    template FUTURE_RETVAL UdpSocket::send_to(Php::Parameters&);
    template FUTURE_RETVAL UdpSocket::recv_from(Php::Parameters&);
    template Php::Value UdpSocket::remote_addr() const;
    template Php::Value UdpSocket::remote_port() const;

    template class Socket<udg>;
    template Php::Value UdgSocket::open_local();
    template Php::Value UdgSocket::bind_local(Php::Parameters&);
    template Future* UdgSocket::recv_from(int64_t, const Php::Value&, const Php::Value&);
    template Future* UdgSocket::send_to(const std::string&, const udg::endpoint&, const Php::Value&, const Php::Value&);
    template FUTURE_RETVAL UdgSocket::send_to(Php::Parameters&);
    template FUTURE_RETVAL UdgSocket::recv_from(Php::Parameters&);
    template Php::Value UdgSocket::remote_path() const;
}
