/**
 * php-asio/include/future.hpp
 *
 * @author CismonX<admin@cismon.net>
 */

#pragma once

#include "common.hpp"
#include "wrapped_handler.hpp"

#define ASYNC_CALLBACK(type) std::function<CORO_RETVAL(const boost::system::error_code&, type)>

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
         * Handler callback of the async operation.
         */
        void* callback_;

#ifdef ENABLE_COROUTINE
        /**
         * Last error code emitted by yielded async operations of this thread.
         */
        static thread_local int64_t last_error_;

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
#endif // ENABLE_COROUTINE

#ifdef ENABLE_STRAND
        /**
         * Pointer to Strand which wrapped this Future.
         */
        boost::asio::strand* strand_ = nullptr;
#endif // ENABLE_STRAND

        /**
         * PHP wrapper for this object.
         */
        Php::Object* wrapper_;

        /**
         * Make default constructor private to avoid user instantiation.
         */
        explicit Future();

    public:
        /**
         * Create a new Future instance.
         */
        static Future* add()
        {
            return new Future;
        }

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
        template <typename T>
        void on_resolve(const ASYNC_CALLBACK(T)&& callback);

        /**
         * Resolve the Future upon operation completion.
         * @param ec : Error code
         * @param arg : Dependent argument
         */
        template <typename T>
        void resolve(const boost::system::error_code& ec, T arg);

#ifdef ENABLE_STRAND
        Php::Value strand(const Php::Value& callback);

        /**
         * Get the pointer to the strand which wrapped this Future.
         */
        boost::asio::strand* get_strand() const
        {
            return strand_;
        }
#endif // ENABLE_STRAND

#ifdef ENABLE_COROUTINE
        /**
         * Attempt to start/resume a coroutine with a PHP Generator.
         */
        static void coroutine(const Php::Value& generator);

        /**
         * Get last error emitted by handler callback within yielded Future.
         */
        static Php::Value get_last_error()
        {
            return last_error_;
        }
#endif // ENABLE_COROUTINE

    };
}
