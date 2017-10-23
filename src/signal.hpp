#pragma once

namespace Asio
{
    class Signal : public Base
    {
        /**
         * Boost.Asio signal_set instance.
         */
        boost::asio::signal_set _signal;

        /**
         * Argument to be passed to signal callback.
         */
        Php::Value _argument;

        /**
         * Signal callback.
         */
        Php::Value _callback;

        /**
         * Whether signal handler is marked as cancelled.
         */
        bool _cancelled = false;

        /**
         * Signal handler.
         * @param error : Error code
         * @param signal : Signal number
         */
        void _handler(const boost::system::error_code& error, int signal);

        /**
         * Async wait.
         */
        void _wait();

    public:

        /**
         * Constructor.
         * @param io_service : IO service for current signal handler
         * @param argument : Argument to be passed to signal callback
         * @param callback : Signal callback
         */
        explicit Signal(
            boost::asio::io_service& io_service,
            const Php::Value& argument,
            const Php::Value& callback
        );

        /**
         * Destructor.
         */
        virtual ~Signal() = default;

        /**
         * Add signals.
         */
        void add(Php::Parameters&);

        /**
         * Internal method for adding signals.
         */
        void add(const std::vector<Php::Value>&& signals);

        /**
         * Remove signals.
         */
        void remove(Php::Parameters&);

        /**
         * Remove all signals.
         */
        void clear();

        /**
         * Cancel current signal handler.
         */
        void cancel();
    };
}
