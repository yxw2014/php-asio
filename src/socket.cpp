/**
 * php-asio/socket.cpp
 *
 * @author CismonX<admin@cismon.net>
 */

#include "socket.hpp"

namespace Asio
{
    template <>
    TcpSocket::Socket(boost::asio::io_service& io_service) : Base(io_service), _socket(io_service)
    {
        _wrapper = new Php::Object("Asio\\TcpSocket", this);
    }

    template <typename protocol, typename socket_type>
    Socket<protocol, socket_type>::~Socket()
    {
        close();
    }

    template <typename protocol, typename socket_type>
    socket_type& Socket<protocol, socket_type>::getSocket()
    {
        return _socket;
    }

    template <typename protocol, typename socket_type>
    void Socket<protocol, socket_type>::_read_handler(
        const boost::system::error_code& error,
        size_t length,
        const Php::Value& callback,
        const Php::Value& argument,
        std::vector<uint8_t>* buffer)
    {
        callback(this, std::string(buffer->begin(), buffer->end()), boost::numeric_cast<int64_t>(length), error.value(), argument);
        delete buffer;
    }

    template <typename protocol, typename socket_type>
    void Socket<protocol, socket_type>::_write_handler(
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

    template <typename protocol, typename socket_type> template <class>
    void Socket<protocol, socket_type>::read(Php::Parameters& params)
    {
        auto length = params[0].numericValue();
        if (length < 0)
            throw Php::Exception("Non-negative integer expected.");
        _read(length, params[1].boolValue(), params[2], params.size() == 3 ? Php::Value() : params[3]);
    }

    template <typename protocol, typename socket_type> template <class>
    void Socket<protocol, socket_type>::write(Php::Parameters& params)
    {
        auto param_count = params.size();
        _write(params[0].stringValue(), params[1].boolValue(), param_count < 3 ? Php::Value() : params[2],
            param_count < 4 ? Php::Value() : params[3]);
    }

    template <typename protocol, typename socket_type>
    Php::Value Socket<protocol, socket_type>::available() const
    {
        return boost::numeric_cast<int64_t>(_socket.available());
    }

    template <typename protocol, typename socket_type>
    Php::Value Socket<protocol, socket_type>::atMark() const
    {
        return _socket.at_mark();
    }

    template <typename protocol, typename socket_type>
    void Socket<protocol, socket_type>::close()
    {
        if (!_closed)
        {
            _closed = true;
            _socket.close();
        }
        if (_wrapper != nullptr)
        {
            delete _wrapper;
            _wrapper = nullptr;
        }
    }

    template <typename protocol, typename socket_type> template <class>
    void Socket<protocol, socket_type>::_read(int64_t length, bool read_some, const Php::Value& callback, const Php::Value& argument)
    {
        if (_closed)
            throw Php::Exception("Connection closed.");
        auto size = boost::numeric_cast<size_t>(length);
        auto buffer_container = new std::vector<uint8_t>(size);
        auto buffer = boost::asio::buffer(*buffer_container, size);
        auto handler = boost::bind(&TcpSocket::_read_handler, this, boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred, callback, argument, buffer_container);
        if (read_some)
            _socket.async_read_some(buffer, handler);
        else
            async_read(_socket, buffer, handler);
    }

    template <typename protocol, typename socket_type> template <class>
    void Socket<protocol, socket_type>::_write(const std::string& data, bool write_some, const Php::Value& callback, const Php::Value& argument)
    {
        if (_closed)
            throw Php::Exception("Connection closed.");
        // Note that `data` gets out of scope when handler callback is called.
        auto buffer = new std::string(data);
        auto handler = boost::bind(&TcpSocket::_write_handler, this, boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred, callback, argument, buffer);
        if (write_some)
            _socket.async_write_some(boost::asio::buffer(*buffer), handler);
        else
            async_write(_socket, boost::asio::buffer(*buffer), handler);
    }

    template class Socket<tcp, tcp::socket>;
}