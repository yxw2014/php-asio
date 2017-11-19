/**
 * php-asio/socket.cpp
 *
 * @author CismonX<admin@cismon.net>
 */

#include "socket.hpp"

namespace Asio
{
    template <typename Protocol>
    Php::Value Socket<Protocol>::read_handler(const boost::system::error_code& error, size_t length, std::vector<uint8_t>* buffer)
    {
        std::string str_buffer(buffer->begin(), buffer->end());
        if (callback_.isCallable())
            Future::coroutine(callback_(this, str_buffer, static_cast<int64_t>(length), error.value(), argument_));
        delete buffer;
        return str_buffer;
    }

    template <typename Protocol>
    Php::Value Socket<Protocol>::write_handler(const boost::system::error_code& error, size_t length, std::string* buffer)
    {
        auto bytes_transferred = static_cast<int64_t>(length);
        if (callback_.isCallable())
            Future::coroutine(callback_(this, bytes_transferred, error.value(), argument_));
        delete buffer;
        return bytes_transferred;
    }

    template <typename Protocol> template <typename, typename>
    Future* Socket<Protocol>::read(int64_t length, bool read_some)
    {
        if (cancelled_)
            throw Php::Exception("Trying to read on a closed socket.");
        auto size = static_cast<size_t>(length);
        auto buffer_container = new std::vector<uint8_t>(size);
        auto buffer = boost::asio::buffer(*buffer_container, size);
        auto future = new Future();
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
        auto future = new Future();
        future->on_resolve<size_t>(boost::bind(&Socket::write_handler, this, _1, _2, buffer));
        if (write_some)
            socket_.async_write_some(boost::asio::buffer(*buffer), ASYNC_HANDLER_DOUBLE_ARG(size_t));
        else
            async_write(socket_, boost::asio::buffer(*buffer), ASYNC_HANDLER_DOUBLE_ARG(size_t));
        return future;
    }

    template <typename Protocol>
    Socket<Protocol>::Socket(boost::asio::io_service& io_service) : Base(io_service), socket_(io_service) {}

    template <>
    void TcpSocket::wrap()
    {
        wrapper_ = new Php::Object("Asio\\TcpSocket", this);
    }

    template <>
    void UnixSocket::wrap()
    {
        wrapper_ = new Php::Object("Asio\\UnixSocket", this);
    }

    template <typename Protocol>
    void Socket<Protocol>::unwrap()
    {
        if (wrapper_ != nullptr) {
            delete wrapper_;
            wrapper_ = nullptr;
        }
    }

    template <typename Protocol>
    typename Protocol::socket& Socket<Protocol>::getSocket()
    {
        return socket_;
    }

    template <typename Protocol> template <typename, typename>
    Php::Value Socket<Protocol>::read(Php::Parameters& params)
    {
        auto length = params[0].numericValue();
        auto param_count = params.size();
        callback_ = param_count < 3 ? Php::Value() : params[2];
        argument_ = param_count < 4 ? Php::Value() : params[3];
        if (length < 0)
            throw Php::Exception("Non-negative integer expected.");
        return read(length, params[1].boolValue());
    }

    template <typename Protocol> template <typename, typename>
    Php::Value Socket<Protocol>::write(Php::Parameters& params)
    {
        auto param_count = params.size();
        callback_ = param_count < 3 ? Php::Value() : params[2];
        argument_ = param_count < 4 ? Php::Value() : params[3];
        return write(params[0].stringValue(), params[1].boolValue());
    }

    template <typename Protocol>
    Php::Value Socket<Protocol>::available(Php::Parameters& params) const
    {
        boost::system::error_code ec;
        auto bytes = socket_.available(ec);
        if (params.size())
            params[0] = static_cast<int64_t>(ec.value());
        return static_cast<int64_t>(bytes);
    }

    template <typename Protocol>
    Php::Value Socket<Protocol>::at_mark(Php::Parameters& params) const
    {
        boost::system::error_code ec;
        auto at_mark = socket_.at_mark(ec);
        if (params.size())
            params[0] = static_cast<int64_t>(ec.value());
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
                unwrap();
            }
        }
        return static_cast<int64_t>(ec.value());
    }

    // Instantiation for TcpSocket.
    template class Socket<boost::asio::ip::tcp>;
    template Php::Value Socket<boost::asio::ip::tcp>::read(Php::Parameters&);
    template Php::Value Socket<boost::asio::ip::tcp>::write(Php::Parameters&);
    template Future* Socket<boost::asio::ip::tcp>::read(int64_t, bool);
    template Future* Socket<boost::asio::ip::tcp>::write(const std::string&, bool);

    // Instantiation for UnixSocket.
    template class Socket<boost::asio::local::stream_protocol>;
    template Php::Value Socket<boost::asio::local::stream_protocol>::read(Php::Parameters&);
    template Php::Value Socket<boost::asio::local::stream_protocol>::write(Php::Parameters&);
    template Future* Socket<boost::asio::local::stream_protocol>::read(int64_t, bool);
    template Future* Socket<boost::asio::local::stream_protocol>::write(const std::string&, bool);
}
