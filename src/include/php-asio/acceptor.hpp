/**
 * php-asio/include/acceptor.hpp
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
    class Acceptor : public Base
    {
        /**
         * Boost.Asio acceptor instance.
         */
        typename Protocol::acceptor acceptor_;

        /**
         * Async accept.
         * @return Future
         */
        Future* accept();

        /**
         * Accept handler.
         * @param error : Error code
         * @param socket : Socket of accepted connection
         * @return Accepted socket
         */
        Php::Value handler(const boost::system::error_code& error, Socket<Protocol>* const socket);

    public:
        /**
         * Constructor.
         * @param io_service : IO service for current Acceptor.
         */
        explicit Acceptor(boost::asio::io_service& io_service);

        /**
         * Default destructor.
         */
        virtual ~Acceptor() = default;

        /**
         * Open INET socket acceptor (TCP).
         */
        template <typename P = Protocol, std::string(P::endpoint::*port)() = nullptr>
        Php::Value open_inet(Php::Parameters&);

        /**
         * Open UNIX domain socket acceptor (SOCK_STREAM).
         */
        template <typename P = Protocol, std::string(P::endpoint::*path)() = nullptr>
        Php::Value open_local();

        /**
         * Bind acceptor to a local endpoint.
         */
        Php::Value bind(Php::Parameters&);

        /**
         * Acceptor start listening so that it could accept connections.
         */
        Php::Value listen(Php::Parameters&);

        /**
         * Accept incoming client connection.
         */
        Php::Value accept(Php::Parameters&);

        /**
         * Cancel async operations and stop Acceptor.
         */
        Php::Value stop();

    };

    // Type alias for Acceptor.
    using TcpAcceptor = Acceptor<tcp>;
    using UnixAcceptor = Acceptor<unix>;
}
