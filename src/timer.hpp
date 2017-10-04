#pragma once

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
        boost::asio::deadline_timer _timer;

        /**
         * Timer interval(milliseconds).
         */
        int64_t _interval;

        /**
         * Argument to be passed to timer callback.
         */
        Php::Value _argument;

        /**
         * Timer callback.
         */
        Php::Value _callback;

        /**
         * Whether timer is persistent.
         */
        bool _persistent;

        /**
         * Boolean flag for execution context.
         */
        bool _context_flag = false;

        /**
         * Defer the timer.
         */
        void _defer();

        /**
         * Handler for timer callback.
         * @param error : error code
         */
        void _handler(const boost::system::error_code& error);

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