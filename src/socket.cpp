/**
 * php-asio/socket.cpp
 *
 * @author CismonX<admin@cismon.net>
 */

#include "socket.hpp"

namespace Asio
{
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

    template <typename Protocol>
    void Socket<Protocol>::read_handler(
        const boost::system::error_code& error,
        size_t length,
        const Php::Value& callback,
        const Php::Value& argument,
        std::vector<uint8_t>* buffer)
    {
        callback(this, std::string(buffer->begin(), buffer->end()), boost::numeric_cast<int64_t>(length), error.value(), argument);
        delete buffer;
    }

    template <typename Protocol>
    void Socket<Protocol>::write_handler(
        const boost::system::error_code& error,
        size_t length,
        const Php::Value& callback,
        const Php::Value& argument,
        std::string* buffer)
    {
        if (callback.isCallable())
            callback(this, boost::numeric_cast<int64_t>(length), error.value(), argument);
        delete buffer;
    }

    template <typename Protocol> template <typename, typename>
    void Socket<Protocol>::read(Php::Parameters& params)
    {
        auto length = params[0].numericValue();
        if (length < 0)
            throw Php::Exception("Non-negative integer expected.");
        read(length, params[1].boolValue(), params[2], params.size() == 3 ? Php::Value() : params[3]);
    }

    template <typename Protocol> template <typename, typename>
    void Socket<Protocol>::write(Php::Parameters& params)
    {
        auto param_count = params.size();
        write(params[0].stringValue(), params[1].boolValue(), param_count < 3 ? Php::Value() : params[2],
            param_count < 4 ? Php::Value() : params[3]);
    }

    template <typename Protocol>
    Php::Value Socket<Protocol>::available() const
    {
        return boost::numeric_cast<int64_t>(socket_.available());
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

    template <typename Protocol> template <typename, typename>
    void Socket<Protocol>::read(int64_t length, bool read_some, const Php::Value& callback, const Php::Value& argument)
    {
        if (closed_)
            return;
        auto size = boost::numeric_cast<size_t>(length);
        auto buffer_container = new std::vector<uint8_t>(size);
        auto buffer = boost::asio::buffer(*buffer_container, size);
        auto handler = boost::bind(&Socket::read_handler, this, boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred, callback, argument, buffer_container);
        if (read_some)
            socket_.async_read_some(buffer, handler);
        else
            async_read(socket_, buffer, handler);
    }

    template <typename Protocol> template <typename, typename>
    void Socket<Protocol>::write(const std::string& data, bool write_some, const Php::Value& callback, const Php::Value& argument)
    {
        if (closed_)
            return;
        // Note that `data` gets out of scope when handler callback is called.
        auto buffer = new std::string(data);
        auto handler = boost::bind(&Socket::write_handler, this, boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred, callback, argument, buffer);
        if (write_some)
            socket_.async_write_some(boost::asio::buffer(*buffer), handler);
        else
            async_write(socket_, boost::asio::buffer(*buffer), handler);
    }

    // Instantiation for TcpSocket.
    template class Socket<boost::asio::ip::tcp>;
    template void Socket<boost::asio::ip::tcp>::read(Php::Parameters&);
    template void Socket<boost::asio::ip::tcp>::write(Php::Parameters&);
    template void Socket<boost::asio::ip::tcp>::read(int64_t, bool, const Php::Value&, const Php::Value&);
    template void Socket<boost::asio::ip::tcp>::write(const std::string&, bool, const Php::Value&, const Php::Value&);

    // Instantiation for UnixSocket.
    template class Socket<boost::asio::local::stream_protocol>;
    template void Socket<boost::asio::local::stream_protocol>::read(Php::Parameters&);
    template void Socket<boost::asio::local::stream_protocol>::write(Php::Parameters&);
    template void Socket<boost::asio::local::stream_protocol>::read(int64_t, bool, const Php::Value&, const Php::Value&);
    template void Socket<boost::asio::local::stream_protocol>::write(const std::string&, bool, const Php::Value&, const Php::Value&);
}