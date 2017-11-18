/**
 * php-asio/include/php-asio/server.hpp
 *
 * @author CismonX<admin@cismon.net>
 */

#pragma once

#include "common.hpp"
#include "base.hpp"
#include "socket.hpp"
#include "future.hpp"

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
        typename Protocol::acceptor* acceptor_ = nullptr;

        /**
         * Async accept.
         */
        Future* accept();

        /**
         * Accept handler.
         * @param error : Error code
         * @param socket : Socket of accepted connection
         */
        Php::Value handler(const boost::system::error_code& error, Socket<Protocol>* const socket);

    public:
        /**
         * Constructor.
         * @param io_service : IO service for current TCP server.
         */
        explicit Server(
            boost::asio::io_service& io_service);

        /**
         * Default destructor.
         */
        virtual ~Server() = default;

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
        Php::Value accept(Php::Parameters&);

        /**
         * Stop server.
         */
        Php::Value stop();

    };

    using TcpServer = Server<boost::asio::ip::tcp>;
    using UnixServer = Server<boost::asio::local::stream_protocol>;
}
