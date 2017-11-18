/**
 * php-asio/include/php-asio/base.hpp
 *
 * @author CismonX<admin@cismon.net>
 */

#pragma once

#include "common.hpp"

namespace Asio
{
    class Base : public Php::Base
    {

    protected:
        /**
         * IO service of this object.
         */
        boost::asio::io_service& io_service_;

        /**
         * Extra argument to be passed to handler callback.
         */
        Php::Value argument_;

        /**
         * Handler callback.
         */
        Php::Value callback_;

        /**
         * True when all async operations have been cancelled and this object is about to destroy.
         */
        bool cancelled_ = false;

        /**
         * PHP wrapper for this object.
         */
        Php::Object* wrapper_ = nullptr;

        /**
         * Constructor.
         * @param io_service : IO service for current object
         */
        explicit Base(boost::asio::io_service& io_service);

        /**
         * Deleted default constructor.
         */
        Base() = delete;

        /**
         * Deleted copy constructor.
         */
        Base(const Base&) = delete;

        /**
         * Deleted copy assignment operator.
         */
        Base& operator=(const Base&) = delete;

    public:
        /**
         * Destructor.
         */
        virtual ~Base() = default;
    };
}
