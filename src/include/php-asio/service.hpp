/**
 * php-asio/include/php-asio/service.hpp
 *
 * @author CismonX<admin@cismon.net>
 */

#pragma once

#include "common.hpp"

namespace Asio
{
    /**
     * Wrapper for Boost.Asio IO service.
     * Provide access to instantiation of IO objects.
     */
    class Service : public Php::Base
    {

        /**
         * The io_service of all IO objects in current instance
         */
        boost::asio::io_service io_service_;

    public:

        /**
         * Default constructor.
         */
        Service() = default;

        /**
         * Deleted copy constructor.
         */
        Service(const Service&) = delete;

        /**
         * Default destructor.
         */
        virtual ~Service() = default;

        /**
         * Deleted copy assignment operator.
         */
        Service& operator=(const Service&) = delete;

        /**
         * Add a new timer.
         * @throws Php::Exception : thrown when timer callback is not callable.
         */
        Php::Value add_timer(Php::Parameters&);

        /**
         * Create a new TCP server and start listening.
         */
        Php::Value add_tcp_server(Php::Parameters&);

        /**
        * Create a new UNIX domain socket server (SOCK_STREAM) and start listening.
        */
        Php::Value add_unix_server(Php::Parameters&);

        /**
         * Add new signal handler.
         */
        Php::Value add_signal(Php::Parameters&);

        /**
         * Start event-loop in block mode.
         */
        Php::Value run();

        /**
         * Excecute at most one handler within the event loop in block mode.
         */
        Php::Value run_one();

        /**
        * Start event loop in non-block mode.
        */
        Php::Value poll();

        /**
        * Excecute at most one handler within the event loop in non-block mode.
        */
        Php::Value poll_one();

        /**
         * Cancel all pending handlers within the event loop.
         */
        void stop();

        /**
         * Reset the io_service before start it again.
         */
        void reset();

        /**
         * Check whether the event loop has stopped.
         */
        Php::Value stopped() const;

        /**
         * Execute a given callback with argument at the next tick.
         */
        void post(Php::Parameters&);

    };
}