/**
 * php-asio/include/php-asio/future.hpp
 *
 * @author CismonX<admin@cismon.net>
 */

#pragma once

#include "common.hpp"

namespace Asio
{
    /**
     * Class Future.
     * When an asynchronous operation completes, its Future will be resolved.
     * And the corresponding coroutine will resume (if Future was yielded by a Generator).
     */
    class Future : public Php::Base
    {
        /**
         * Last error code emitted by yielded async operations of this thread.
         */
        static thread_local int64_t last_error_;

        /**
         * Handler callback of the async operation.
         */
        std::function<Php::Value(const boost::system::error_code&, unsigned)> callback_;

        /**
         * Generator instance which yielded this Future.
         */
        Php::Value generator_;

        /**
         * Value which will be sent back to the Generator.
         */
        Php::Value send_;

        /**
         * Whether this future is yielded by a Generator.
         */
        bool yield_ = false;

        /**
         * PHP wrapper for this object.
         */
        Php::Object* wrapper_;

    public:
        /**
         * Constructor.
         * @param callback : Handler callback
         */
        explicit Future(const std::function<Php::Value(const boost::system::error_code&, unsigned)>&& callback);

        /**
         * Deleted default constructor.
         */
        Future() = delete;

        /**
         * Deleted copy constructor.
         */
        Future(const Future&) = delete;

        /**
         * Deleted copy assignment operator.
         */
        Future& operator=(const Future&) = delete;

        /**
         * Default destructor.
         */
        virtual ~Future() = default;

        /**
         * Resolve the Future upon operation completion.
         * @param ec : Error code
         * @param arg : Dependent argument
         */
        void resolve(const boost::system::error_code& ec, unsigned arg);

        /**
         * Attempt to start/resume a coroutine with a PHP Generator.
         */
        static void coroutine(const Php::Value& generator);

        /**
         * Get last error emitted by handler callback within yielded Future.
         */
        static Php::Value get_last_error();
    };
}
