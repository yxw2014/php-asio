/**
 * php-asio/include/php-asio/server.hpp
 *
 * @author CismonX<admin@cismon.net>
 */

#pragma once

#include "common.hpp"
#include "base.hpp"
#include "socket.hpp"

namespace Asio
{
    /**
     * Wrapper for Boost.Asio stream socket acceptor.
     * Provide TCP services.
     */
    template <typename Protocol>
    class Server : public Base
    {
        /**
         * Acceptor for this server.
         */
        typename Protocol::acceptor* acceptor_;

        /**
         * Argument to be passed to acceptor callback.
         */
        Php::Value argument_;

        /**
         * Timer callback.
         */
        Php::Value callback_;

        /**
         * Whether the server start accepting once created.
         */
        bool auto_accept_;

        /**
         * Whether the server is marked as stopped.
         */
        bool stopped_ = false;

        /**
         * Boolean flag for execution context.
         */
        bool context_flag_ = false;

        /**
         * Async accept.
         * 
         * The void* argument is to prevent ambiguous call when adding method.
         */
        void accept(void* = nullptr);

        /**
         * Accept handler.
         * @param error : Error code
         * @param socket : Client connection
         */
        void _handler(const boost::system::error_code& error, Socket<Protocol>* const socket);

    public:
        /**
         * Constructor.
         * @param io_service : IO service for current TCP server.
         * @param auto_accept : Whether to start accepting once server is created.
         * @param argument : Extra argument to be passed to acceptor callback.
         * @param callback : Acceptor callback.
         */
        explicit Server(
            boost::asio::io_service& io_service,
            bool auto_accept,
            const Php::Value& argument,
            const Php::Value& callback);

        /**
         * Destructor.
         */
        virtual ~Server();

        /**
         * Init acceptor for IP-based sockets.
         * @param address : Address which the server will bind to.
         * @param port : Port which the server will bind to.
         */
        void init_acceptor(const std::string& address, unsigned short port);

        /**
         * Init acceptor for local sockets.
         * @param path : Socket path.
         */
        void init_acceptor(const std::string& path);

        /**
         * Accept incoming client connection once.
         */
        void accept(Php::Parameters&);

        /**
         * Stop server.
         */
        void stop();

    };

    using TcpServer = Server<boost::asio::ip::tcp>;
    using UnixServer = Server<boost::asio::local::stream_protocol>;
}