/**
 * php-asio/include/wrapped_handler.hpp
 *
 * @author CismonX<admin@cismon.net>
 */

#pragma once

#include "common.hpp"

#ifdef ENABLE_STRAND
namespace Asio
{
    /**
     * Async handler callback wrapped by strand.
     */
    class WrappedHandler : public Php::Base
    {
        /**
         * Strand object for this wrapped handler.
         */
        boost::asio::strand& strand_;

        /**
         * Wrapped handler callback.
         */
        Php::Value callback_;

        /**
         * Future has access to Strand and handler callback.
         */
        friend class Future;
        
    public:
        /**
         * Constructor.
         * @param strand : Strand object for this wrapped handler
         * @param callback : Wrapped handler callback
         */
        explicit WrappedHandler(
            boost::asio::strand& strand,
            const Php::Value& callback
        ) : strand_(strand), callback_(callback) {}

        /**
         * Deleted default constructor.
         */
        WrappedHandler() = delete;

        /**
         * Deleted copy constructor.
         */
        WrappedHandler(const WrappedHandler&) = delete;

        /**
         * Make this object callable.
         */
        void __invoke(Php::Parameters&) const {}
    };

}
#endif // ENABLE_STRAND
