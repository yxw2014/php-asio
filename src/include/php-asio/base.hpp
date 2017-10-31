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
        boost::asio::io_service& _io_service;

        /**
         * PHP wrapper for this object.
         */
        Php::Object* _wrapper;

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

        /**
         * Destructor.
         */
        virtual ~Base() = default;
    };
}