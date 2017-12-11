/**
 * php-asio/include/strand.hpp
 *
 * @author CismonX<admin@cismon.net>
 */

#pragma once

#include "common.hpp"
#include "future.hpp"

#ifdef ENABLE_STRAND
namespace Asio
{
    /**
     * Wrapper for Boost.Asio strand.
     * Provides serialised handler execution.
     */
    class Strand : public Php::Base
    {
        /**
         * Boost.Asio strand instance.
         */
        boost::asio::strand strand_;

        /**
         * PHP Wrapper for this object.
         */
        Php::Object* wrapper_;

    public:
        /**
         * Constructor.
         * @param io_service : I/O service for strand
         */
        explicit Strand(boost::asio::io_service& io_service);

        /**
         * Deleted default constructor.
         */
        Strand() = delete;

        /**
         * Deleted copy constructor.
         */
        Strand(const Strand&) = delete;

        /**
         * Request the strand to invoke the given handler.
         */
        void dispatch(Php::Parameters&);

        /**
         * Request the strand to invoke the given handler and return immediately.
         */
        void post(Php::Parameters&);

        /**
         * Determine whether the strand is running in the current thread.
         */
        Php::Value running_in_this_thread() const;

        /**
         * Create a new handler that automatically dispatches the wrapped handler on the strand.
         */
        Php::Value wrap(Php::Parameters&);

        /**
         * Destroy current strand object.
         */
        void destroy();
    };
}
#endif // ENABLE_STRAND
