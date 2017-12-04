/**
 * php-asio/include/resolver.hpp
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
     * Wrapper for Boost.Asio resolver.
     * Provides hostname resolution.
     */
    template <typename Protocol>
    class Resolver : public Base
    {
        /**
         * Resolver iterator which holds an endpoint.
         */
        using iterator = typename Protocol::resolver::iterator;

        /**
         * Boost.Asio resolver instance.
         */
        typename Protocol::resolver resolver_;

        /**
         * Resolve handler.
         * @param error : Error code
         * @param iter : Endpoint iterator
         */
        CORO_RETVAL handler(
            const boost::system::error_code& error,
            iterator iter,
            const Php::Value& callback,
            const Php::Value& argument);

        /**
         * The internal resolve method.
         * @return Future
         */
        Future* resolve(
            const typename Protocol::resolver::query&& query,
            const Php::Value& callback,
            const Php::Value& argument);

    public:
        /**
         * Constructor.
         * @param io_service : I/O service for current Resolver.
         */
        explicit Resolver(boost::asio::io_service& io_service);

        /**
         * Default destructor.
         */
        virtual ~Resolver() = default;

        /**
         * Initiate an asynchronous resolve against the resolver.
         */
        FUTURE_RETVAL resolve(Php::Parameters&);

        /**
         * Cancel resolve operation and destroy the resolver.
         */
        void cancel();
    };

    // Type alias for Resolver.
    using TcpResolver = Resolver<tcp>;
    using UdpResolver = Resolver<udp>;
}
