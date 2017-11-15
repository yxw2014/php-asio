/**
 * php-asio/include/php-asio/socket.hpp
 * 
 * @author CismonX<admin@cismon.net>
 */

#pragma once

#include "common.hpp"
#include "base.hpp"
#include "future.hpp"

namespace Asio
{
    /**
     * Wrapper for Boost.Asio stream socket.
     */
    template <typename Protocol>
    class Socket : public Base
    {
        /**
         * Current socket.
         */
        typename Protocol::socket socket_;

        /**
         * Whether socket is marked as closed.
         */
        bool closed_ = false;

        /**
         * Read/recieve handler for socket.
         * @param error : Error code
         * @param length : Bytes transferred
         * @param callback : Handler callback
         * @param argument : Extra argument
         * @param buffer : Read buffer
         */
        Php::Value read_handler(
            const boost::system::error_code& error,
            size_t length,
            const Php::Value& callback,
            const Php::Value& argument,
            std::vector<uint8_t>* buffer);

        /**
         * Write/send handler for socket.
         * @param error : Error code
         * @param length : Bytes transferred
         * @param callback : Handler callback
         * @param argument : Extra argument
         * @param buffer : Write buffer
         */
        Php::Value write_handler(
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
        template<typename P = Protocol, typename = typename std::enable_if<
            std::is_same<boost::asio::basic_stream_socket<P>, typename P::socket>::value>::type>
        Future* read(int64_t length, bool read_some, const Php::Value& callback, const Php::Value& argument);

        /**
         * The internal write method.
         * @param data : Data to write to stream socket.
         * @param write_some : If false, handler will not be called unless all data is sent.
         * @param callback : Read handler callback
         * @param argument : Extra argument
         */
        template<typename P = Protocol, typename = typename std::enable_if<
            std::is_same<boost::asio::basic_stream_socket<P>, typename P::socket>::value>::type>
        Future* write(const std::string& data, bool write_some, const Php::Value& callback, const Php::Value& argument);

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
         * Wrap socket in Php::Object.
         */
        void wrap();

        /**
         * Unwrap socket.
         */
        void unwrap();

        /**
         * Get reference of socket.
         */
        typename Protocol::socket& getSocket(); 

        /**
         * Read asynchronously from stream socket.
         */
        template<typename P = Protocol, typename = typename std::enable_if<
            std::is_same<boost::asio::basic_stream_socket<P>, typename P::socket>::value>::type>
        Php::Value read(Php::Parameters&);

        /**
         * Write asynchronously to stream socket.
         */
        template<typename P = Protocol, typename = typename std::enable_if<
            std::is_same<boost::asio::basic_stream_socket<P>, typename P::socket>::value>::type>
        Php::Value write(Php::Parameters&);

        /**
         * Determine the number of bytes available for reading.
         */
        Php::Value available() const;

        /**
         * Determine whether the socket is at the out-of-band data mark.
         */
        Php::Value at_mark() const;

        /**
         * Close socket.
         */
        void close();

    };

    using TcpSocket = Socket<boost::asio::ip::tcp>;
    using UnixSocket = Socket<boost::asio::local::stream_protocol>;
}