/**
 * php-asio/socket.cpp
 *
 * @author CismonX<admin@cismon.net>
 */

#include "socket.hpp"

namespace Asio
{
    template <typename Protocol>
    Php::Value Socket<Protocol>::read_handler(
        const boost::system::error_code& error,
        size_t length,
        const Php::Value& callback,
        const Php::Value& argument,
        std::vector<uint8_t>* buffer)
    {
        std::string str_buffer(buffer->begin(), buffer->end());
        if (callback.isCallable())
            Future::coroutine(callback(this, str_buffer, static_cast<int64_t>(length), error.value(), argument));
        delete buffer;
        return str_buffer;
    }

    template <typename Protocol>
    Php::Value Socket<Protocol>::write_handler(
        const boost::system::error_code& error,
        size_t length,
        const Php::Value& callback,
        const Php::Value& argument,
        std::string* buffer)
    {
        auto bytes_transferred = static_cast<int64_t>(length);
        if (callback.isCallable())
            Future::coroutine(callback(this, bytes_transferred, error.value(), argument));
        delete buffer;
        return bytes_transferred;
    }

    template <typename Protocol> template <typename, typename>
    Future* Socket<Protocol>::read(int64_t length, bool read_some, const Php::Value& callback, const Php::Value& argument)
    {
        if (closed_)
            throw Php::Exception("Trying to read on a closed socket.");
        auto size = static_cast<size_t>(length);
        auto buffer_container = new std::vector<uint8_t>(size);
        auto buffer = boost::asio::buffer(*buffer_container, size);
        auto future = new Future(boost::bind(&Socket::read_handler, this, _1, _2, callback, argument, buffer_container));
        if (read_some)
            socket_.async_read_some(buffer, PHP_ASIO_ASYNC_HANDLER(boost::asio::placeholders::bytes_transferred));
        else
            async_read(socket_, buffer, PHP_ASIO_ASYNC_HANDLER(boost::asio::placeholders::bytes_transferred));
        return future;
    }

    template <typename Protocol> template <typename, typename>
    Future* Socket<Protocol>::write(const std::string& data, bool write_some, const Php::Value& callback, const Php::Value& argument)
    {
        if (closed_)
            throw Php::Exception("Trying to write to a closed socket.");
        // Note that `data` gets out of scope when handler callback is called.
        auto buffer = new std::string(data);
        auto future = new Future(boost::bind(&Socket::write_handler, this, _1, _2, callback, argument, buffer));
        if (write_some)
            socket_.async_write_some(boost::asio::buffer(*buffer), PHP_ASIO_ASYNC_HANDLER(boost::asio::placeholders::bytes_transferred));
        else
            async_write(socket_, boost::asio::buffer(*buffer), PHP_ASIO_ASYNC_HANDLER(boost::asio::placeholders::bytes_transferred));
        return future;
    }

    template <typename Protocol>
    Socket<Protocol>::Socket(boost::asio::io_service& io_service) : Base(io_service), socket_(io_service) {}

    template <typename Protocol>
    Socket<Protocol>::~Socket()
    {
        close();
    }

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
        if (wrapper_ != nullptr)
        {
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
        if (length < 0)
            throw Php::Exception("Non-negative integer expected.");
        return read(length, params[1].boolValue(), params[2], params.size() == 3 ? Php::Value() : params[3]);
    }

    template <typename Protocol> template <typename, typename>
    Php::Value Socket<Protocol>::write(Php::Parameters& params)
    {
        auto param_count = params.size();
        return write(params[0].stringValue(), params[1].boolValue(), param_count < 3 ? Php::Value() : params[2],
            param_count < 4 ? Php::Value() : params[3]);
    }

    template <typename Protocol>
    Php::Value Socket<Protocol>::available() const
    {
        return static_cast<int64_t>(socket_.available());
    }

    template <typename Protocol>
    Php::Value Socket<Protocol>::at_mark() const
    {
        return socket_.at_mark();
    }

    template <typename Protocol>
    void Socket<Protocol>::close()
    {
        if (!closed_)
        {
            closed_ = true;
            socket_.cancel();
            socket_.close();
        }
    }

    // Instantiation for TcpSocket.
    template class Socket<boost::asio::ip::tcp>;
    template Php::Value Socket<boost::asio::ip::tcp>::read(Php::Parameters&);
    template Php::Value Socket<boost::asio::ip::tcp>::write(Php::Parameters&);
    template Future* Socket<boost::asio::ip::tcp>::read(int64_t, bool, const Php::Value&, const Php::Value&);
    template Future* Socket<boost::asio::ip::tcp>::write(const std::string&, bool, const Php::Value&, const Php::Value&);

    // Instantiation for UnixSocket.
    template class Socket<boost::asio::local::stream_protocol>;
    template Php::Value Socket<boost::asio::local::stream_protocol>::read(Php::Parameters&);
    template Php::Value Socket<boost::asio::local::stream_protocol>::write(Php::Parameters&);
    template Future* Socket<boost::asio::local::stream_protocol>::read(int64_t, bool, const Php::Value&, const Php::Value&);
    template Future* Socket<boost::asio::local::stream_protocol>::write(const std::string&, bool, const Php::Value&, const Php::Value&);
}