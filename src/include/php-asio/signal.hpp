/**
 * php-asio/include/php-asio/signal.hpp
 *
 * @author CismonX<admin@cismon.net>
 */

#pragma once

#include "common.hpp"
#include "base.hpp"
#include "future.hpp"

namespace Asio
{
    class Signal : public Base
    {
        /**
         * Boost.Asio signal_set instance.
         */
        boost::asio::signal_set signal_;

        /**
         * Argument to be passed to signal callback.
         */
        Php::Value argument_;

        /**
         * Signal callback.
         */
        Php::Value callback_;

        /**
         * Whether signal handler is marked as cancelled.
         */
        bool cancelled_ = false;

        /**
         * Signal handler.
         * @param error : Error code
         * @param signal : Signal number
         */
        Php::Value handler(const boost::system::error_code& error, int signal);

        /**
         * Async wait.
         */
        Future* wait();

        /**
         * Internal method for adding signals.
         */
        int64_t add(const std::vector<Php::Value>&& signals);

    public:
        /**
         * Constructor.
         * @param io_service : IO service for current signal handler
         */
        explicit Signal(boost::asio::io_service& io_service);

        /**
         * Destructor.
         */
        virtual ~Signal() = default;

        /**
         * Add signals.
         */
        Php::Value add(Php::Parameters&);

        /**
         * Remove signals.
         */
        Php::Value remove(Php::Parameters&);

        /**
         * Wait for a signal.
         */
        Php::Value wait(Php::Parameters&);

        /**
         * Remove all signals.
         */
        Php::Value clear();

        /**
         * Cancel current signal handler.
         */
        Php::Value cancel();

    };
}
