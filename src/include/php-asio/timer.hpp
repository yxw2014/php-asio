/**
 * php-asio/include/php-asio/timer.hpp
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
        Future* wait();

        /**
         * Handler for timer callback.
         * @param error : error code
         */
        Php::Value handler(const boost::system::error_code& error);

    public:
        /**
         * Timer constructor.
         * @param io_service : IO service for current timer
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
         * Wait for timer to expire.
         */
        Php::Value wait(Php::Parameters& params);

        /**
         * Cancel timer.
         */
        Php::Value cancel();

    };
}