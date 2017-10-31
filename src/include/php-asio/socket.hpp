/**
 * php-asio/include/php-asio/socket.hpp
 * 
 * @author CismonX<admin@cismon.net>
 */

#pragma once

#include "common.hpp"
#include "base.hpp"

using boost::asio::ip::tcp;

namespace Asio
{
    /**
     * Wrapper for Boost.Asio stream socket.
     */
    template<typename protocol, typename socket_type>
    class Socket : public Base
    {

        /**
         * Current TCP socket.
         */
        socket_type _socket;

        /**
         * Whether socket is marked as closed.
         */
        bool _closed = false;

        /**
         * Read handler for TCP socket.
         * @param error : Error code
         * @param length : Bytes transferred
         * @param callback : Handler callback
         * @param argument : Extra argument
         * @param buffer : Read buffer
         */
        void _read_handler(
            const boost::system::error_code& error,
            size_t length,
            const Php::Value& callback,
            const Php::Value& argument,
            std::vector<uint8_t>* buffer);

        /**
         * Write handler for TCP socket.
         * @param error : Error code
         * @param length : Bytes transferred
         * @param callback : Handler callback
         * @param argument : Extra argument
         * @param buffer : Write buffer
         */
        void _write_handler(
            const boost::system::error_code& error,
            size_t length,
            const Php::Value& callback,
            const Php::Value& argument,
            std::string* buffer);

        /**
         * The internal read method.
         * @param length : Maximum number fo bytes to read
         * @param read_some : If false, handler will not be called unless the required amount of bytes are recieved.
         * @param callback : Read handler callback
         * @param argument : Extra argument
         */
        template<class = typename std::enable_if<
            std::is_same<boost::asio::basic_stream_socket<protocol>, socket_type>::value, socket_type>::type>
        void _read(int64_t length, bool read_some, const Php::Value& callback, const Php::Value& argument);

        /**
         * The internal write method.
         * @param data : Data to write to stream socket.
         * @param write_some : If false, handler will not be called unless all data is sent.
         * @param callback : Read handler callback
         * @param argument : Extra argument
         */
        template<class = typename std::enable_if<
            std::is_same<boost::asio::basic_stream_socket<protocol>, socket_type>::value, socket_type>::type>
        void _write(const std::string& data, bool write_some, const Php::Value& callback, const Php::Value& argument);

    public:

        /**
         * Constructor.
         * @param io_service : IO service of current TCP socket.
         */
        explicit Socket(boost::asio::io_service& io_service);

        /**
         * Default destructor.
         */
        virtual ~Socket();

        /**
         * Get reference of TCP socket.
         */
        socket_type& getSocket(); 

        /**
         * Read asynchronously from stream socket.
         */
        template<class = typename std::enable_if<
            std::is_same<boost::asio::basic_stream_socket<protocol>, socket_type>::value, socket_type>::type>
        void read(Php::Parameters&);

        /**
         * Write asynchronously to stream socket.
         */
        template<class = typename std::enable_if<
            std::is_same<boost::asio::basic_stream_socket<protocol>, socket_type>::value, socket_type>::type>
        void write(Php::Parameters&);

        /**
         * Determine the number of bytes available for reading.
         */
        Php::Value available() const;

        /**
         * Determine whether the socket is at the out-of-band data mark.
         */
        Php::Value atMark() const;

        /**
         * Close the TCP socket.
         */
        void close();

    };

    using TcpSocket = Socket<tcp, tcp::socket>;
}