/**
 * php-asio/include/timer.hpp
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
     * Wrapper for Boost.Asio deadline timer.
     */
    class Timer : public Base
    {
        /**
         * Boost.Asio timer instance.
         */
        boost::asio::deadline_timer timer_;

        /**
         * Defer the timer.
         */
        Future* wait(const Php::Value& callback, const Php::Value& argument);

        /**
         * Handler for timer callback.
         * @param error : error code
         */
        CORO_RETVAL handler(
            const boost::system::error_code& error,
            const Php::Value& callback,
            const Php::Value& argument);

    public:
        /**
         * Timer constructor.
         * @param io_service : I/O service for current timer
         */
        explicit Timer(boost::asio::io_service& io_service);

        /**
         * Timer destructor.
         */
        virtual ~Timer() = default;

        /**
         * Set timer expiry time.
         */
        Php::Value expire(Php::Parameters& params);

        /**
         * Initiate an asynchronous wait against the timer.
         */
        FUTURE_RETVAL wait(Php::Parameters& params);

        /**
         * Cancel timer.
         */
        Php::Value cancel();

    };
}