/**
 * php-asio/include/socket.hpp
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
     * Wrapper for Boost.Asio socket.
     */
    template <typename Protocol>
    class Socket : public Base
    {
        /**
         * Boost.Asio socket instance.
         */
        typename Protocol::socket socket_;

        /**
         * Connect handler.
         * @param error : Error code
         */
        CORO_RETVAL connect_handler(
            const boost::system::error_code& error,
            const Php::Value& callback,
            const Php::Value& argument);

        /**
         * Read handler for stream socket.
         * @param error : Error code
         * @param length : Bytes transferred
         * @param buffer : Read buffer
         */
        template <typename P = Protocol, typename = typename std::enable_if<
            std::is_same<boost::asio::basic_stream_socket<P>, typename P::socket>::value>::type>
        CORO_RETVAL read_handler(
            const boost::system::error_code& error,
            size_t length,
            std::vector<uint8_t>* buffer,
            const Php::Value& callback,
            const Php::Value& argument);

        /**
         * Write/send handler for socket.
         * @param error : Error code
         * @param length : Bytes transferred
         * @param buffer : Write buffer
         */
        CORO_RETVAL write_handler(
            const boost::system::error_code& error,
            size_t length,
            std::string* buffer,
            const Php::Value& callback,
            const Php::Value& argument);

        /**
         * Receive handler for datagram socket.
         * @param error : Error code
         * @param length : Bytes transferred
         * @param buffer : Write buffer
         */
        CORO_RETVAL recv_handler(
            const boost::system::error_code& error,
            size_t length,
            std::vector<uint8_t>* buffer,
            typename Protocol::endpoint* endpoint,
            const Php::Value& callback,
            const Php::Value& argument);

        /**
         * The internal connect method (for Unix domain sockets).
         * @param socket_path : Path to socket file of remote endpoint
         * @return Future
         */
        template <typename P = Protocol, std::string(P::endpoint::*path)() = nullptr>
        Future* connect(
            const std::string& socket_path,
            const Php::Value& callback,
            const Php::Value& argument);

        /**
         * The internal connect method (for INET sockets).
         * @param address : Remote address
         * @param port_num : Remote port
         * @return Future
         */
        template <typename P = Protocol, std::string(P::endpoint::*port)() = nullptr>
        Future* connect(
            const std::string& address,
            unsigned short port_num,
            const Php::Value& callback,
            const Php::Value& argument);

        /**
         * The internal read method.
         * @param length : Maximum number fo bytes to read
         * @param read_some : If false, handler will not be called unless the required amount of bytes are recieved
         * @return Future
         */
        template <typename P = Protocol, typename = typename std::enable_if<
            std::is_same<boost::asio::basic_stream_socket<P>, typename P::socket>::value>::type>
        Future* read(int64_t length, bool read_some, const Php::Value& callback, const Php::Value& argument);

        /**
         * The internal write method.
         * @param data : Data to write to stream socket
         * @param write_some : If false, handler will not be called unless all data is sent
         * @return Future
         */
        template <typename P = Protocol, typename = typename std::enable_if<
            std::is_same<boost::asio::basic_stream_socket<P>, typename P::socket>::value>::type>
        Future* write(const std::string& data, bool write_some, const Php::Value& callback, const Php::Value& argument);

        /**
         * The internal recv_from method.
         * @param length : Maximum number of bytes to be received
         */
        template <typename P = Protocol, typename = typename std::enable_if<
            std::is_same<boost::asio::basic_datagram_socket<P>, typename P::socket>::value>::type>
        Future* recv_from(int64_t length, const Php::Value& callback, const Php::Value& argument);

        /**
         * The internal send_to method.
         * @param data : Data to send to datagram socket
         * @param endpoint : Remote endpoint
         */
        template <typename P = Protocol, typename = typename std::enable_if<
            std::is_same<boost::asio::basic_datagram_socket<P>, typename P::socket>::value>::type>
        Future* send_to(
            const std::string& data,
            const typename P::endpoint& endpoint,
            const Php::Value& callback,
            const Php::Value& argument);

    public:
        /**
         * Constructor.
         * @param io_service : I/O service of current socket.
         */
        explicit Socket(boost::asio::io_service& io_service);

        /**
         * Default destructor.
         */
        virtual ~Socket() = default;

        /**
         * Get reference of socket.
         */
        typename Protocol::socket& getSocket();

        /**
         * Open INET socket (AF_INET or AF_INET6).
         */
        template <typename P = Protocol, std::string(P::endpoint::*port)() = nullptr>
        Php::Value open_inet(Php::Parameters&);

        /**
         * Open UNIX domain socket.
         */
        template <typename P = Protocol, std::string(P::endpoint::*path)() = nullptr>
        Php::Value open_local();

        /**
         * Bind socket to a local endpoint (AF_INET or AF_INET6).
         */
        template <typename P = Protocol, std::string(P::endpoint::*port)() = nullptr>
        Php::Value bind_inet(Php::Parameters&);

        /**
         * Bind socket to a local endpoint (AF_UNIX).
         */
        template <typename P = Protocol, std::string(P::endpoint::*path)() = nullptr>
        Php::Value bind_local(Php::Parameters&);

        /**
         * Connect to a remote endpoint.
         */
        FUTURE_RETVAL connect(Php::Parameters&);

        /**
         * Read asynchronously from stream socket.
         */
        template <typename P = Protocol, typename = typename std::enable_if<
            std::is_same<boost::asio::basic_stream_socket<P>, typename P::socket>::value>::type>
        FUTURE_RETVAL read(Php::Parameters&);

        /**
         * Write asynchronously to stream socket.
         */
        template <typename P = Protocol, typename = typename std::enable_if<
            std::is_same<boost::asio::basic_stream_socket<P>, typename P::socket>::value>::type>
        FUTURE_RETVAL write(Php::Parameters&);

        /**
         * Receive asynchronously to datagram socket.
         */
        template <typename P = Protocol, typename = typename std::enable_if<
            std::is_same<boost::asio::basic_datagram_socket<P>, typename P::socket>::value>::type>
        FUTURE_RETVAL recv_from(Php::Parameters&);

        /**
         * Send asynchronously to datagram socket.
         */
        template <typename P = Protocol, typename = typename std::enable_if<
            std::is_same<boost::asio::basic_datagram_socket<P>, typename P::socket>::value>::type>
        FUTURE_RETVAL send_to(Php::Parameters&);

        /**
         * Get remote address (AF_INET or AF_INET6).
         */
        template <typename P = Protocol, std::string(P::endpoint::*port)() = nullptr>
        Php::Value remote_addr() const;

        /**
         * Get remote port (AF_INET or AF_INET6).
         */
        template <typename P = Protocol, std::string(P::endpoint::*port)() = nullptr>
        Php::Value remote_port() const;

        /**
         * Get socket file path of remote endpoint (AF_UNIX).
         */
        template <typename P = Protocol, std::string(P::endpoint::*path)() = nullptr>
        Php::Value remote_path() const;

        /**
         * Determine the number of bytes available for reading.
         */
        Php::Value available(Php::Parameters&) const;

        /**
         * Determine whether the socket is at the out-of-band data mark.
         */
        Php::Value at_mark(Php::Parameters&) const;

        /**
         * Cancel async operations and close socket.
         */
        Php::Value close();

    };

    using TcpSocket = Socket<tcp>;
    using UnixSocket = Socket<unix>;
    using UdpSocket = Socket<udp>;
    using UdgSocket = Socket<udg>;
}
