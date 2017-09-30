#pragma once

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
        boost::asio::io_service _io_service;

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
        Php::Value addTimer(Php::Parameters&);

        /**
         * Create a new TCP server and start listening.
         */
        Php::Value addTcpServer(Php::Parameters&);

        /**
         * Start event-loop in block mode.
         */
        Php::Value run();

        /**
         * Excecute at most one handler within the event loop in block mode.
         */
        Php::Value runOne();

        /**
        * Start event loop in non-block mode.
        */
        Php::Value poll();

        /**
        * Excecute at most one handler within the event loop in non-block mode.
        */
        Php::Value pollOne();

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