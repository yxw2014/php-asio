/**
 * php-asio/include/php-asio/future.hpp
 *
 * @author CismonX<admin@cismon.net>
 */

#pragma once

#include "common.hpp"

#define ASYNC_CALLBACK(type) std::function<Php::Value(const boost::system::error_code, type)>

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
        void* callback_;

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
         */
        explicit Future();

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
         * Set future resolver callback.
         */
        template <typename Arg>
        void on_resolve(const ASYNC_CALLBACK(Arg)&& callback);

        /**
         * Resolve the Future upon operation completion.
         * @param ec : Error code
         * @param arg : Dependent argument
         */
        template <typename Arg>
        void resolve(const boost::system::error_code& ec, Arg arg);

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
