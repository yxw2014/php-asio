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
        Future* accept(const Php::Value& callback, const Php::Value& argument);

        /**
         * Accept handler.
         * @param error : Error code
         * @param socket : Socket of accepted connection
         */
        CORO_RETVAL handler(
            const boost::system::error_code& error,
            Socket<Protocol>* const socket,
            const Php::Value& callback,
            const Php::Value& argument);

    public:
        /**
         * Constructor.
         * @param io_service : I/O service for current Acceptor.
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
         * Bind the acceptor to the specified local endpoint.
         */
        Php::Value bind(Php::Parameters&);

        /**
         * Put the acceptor into the state where it may accept new connections.
         */
        Php::Value listen(Php::Parameters&);

        /**
         * Asynchronously accept a new connection into a socket.
         */
        FUTURE_RETVAL accept(Php::Parameters&);

        /**
         * Cancel async operations and stop acceptor.
         */
        Php::Value close();

    };

    using TcpAcceptor = Acceptor<tcp>;
    using UnixAcceptor = Acceptor<unix>;
}
