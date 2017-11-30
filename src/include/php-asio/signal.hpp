/**
 * php-asio/include/signal.hpp
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
     * Wrapper for Boost.Asio signal_set.
     * Provides functionalities for signal handling.
     */
    class Signal : public Base
    {
        /**
         * Boost.Asio signal_set instance.
         */
        boost::asio::signal_set signal_;

        /**
         * Signal handler callback.
         * @param error : Error code
         * @param signal : Signal number
         */
        Php::Value handler(const boost::system::error_code& error, int signal);

        /**
         * The internal wait mathod.
         */
        Future* wait();

    public:
        /**
         * Constructor.
         * @param io_service : I/O service for current signal set
         */
        explicit Signal(boost::asio::io_service& io_service);

        /**
         * Default destructor.
         */
        virtual ~Signal() = default;

        /**
         * Add the specified signal(s) to the signal set.
         */
        Php::Value add(Php::Parameters&);

        /**
         * Remove the specified signal(s) from the signal set.
         */
        Php::Value remove(Php::Parameters&);

        /**
         * Initiate an asynchronous wait against the signal set.
         */
        Php::Value wait(Php::Parameters&);

        /**
         * Remove all signals from the signal set.
         */
        Php::Value clear();

        /**
         * Cancel current signal set.
         */
        Php::Value cancel();

    };
}
