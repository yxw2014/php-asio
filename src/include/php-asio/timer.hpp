/**
 * php-asio/include/php-asio/timer.hpp
 *
 * @author CismonX<admin@cismon.net>
 */

#pragma once

#include "common.hpp"
#include "base.hpp"

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
         * Timer interval(milliseconds).
         */
        int64_t interval_;

        /**
         * Argument to be passed to timer callback.
         */
        Php::Value argument_;

        /**
         * Timer callback.
         */
        Php::Value callback_;

        /**
         * Whether timer is persistent.
         */
        bool persistent_;

        /**
         * Boolean flag for execution context.
         */
        bool context_flag_ = false;

        /**
         * Defer the timer.
         * 
         * The void* argument is to prevent ambiguous call when adding method.
         */
        void defer(void* = nullptr);

        /**
         * Handler for timer callback.
         * @param error : error code
         */
        void handler(const boost::system::error_code& error);

    public:
        /**
         * Timer constructor.
         * @param io_service : IO service for current timer
         * @param interval : Timer interval
         * @param argument : Argument to be passed to timer callback
         * @param callback : Timer callback
         * @param persistent : Whether timer repeats
         */
        explicit Timer(
            boost::asio::io_service& io_service,
            int64_t interval,
            const Php::Value& argument,
            const Php::Value& callback,
            bool persistent);

        /**
         * Timer destructor.
         */
        virtual ~Timer() = default;

        /**
         * Set next expire time for timer.
         */
        void defer(Php::Parameters& params);

        /**
         * Set timer's persistant attribute to false.
         */
        void cancel();

    };
}