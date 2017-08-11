#pragma once

/**
 * PHP-CPP Wrapper for Boost.Asio
 * This is the only class which is exposed to PHP.
 * See export.php for details of function parameters.
 */
class Asio : public Php::Base
{
    /**
     * The io_service of all IO objects in current instance
     */
    IoService _io_service;

public:

    /**
     * Add a new timer.
     * @throws Php::Exception : thrown when timer callback is not callable.
     */
    Php::Value addTimer(Php::Parameters &);

    /**
     * Cancel a timer and destroy all resource it allocates.
     */
    void delTimer(Php::Parameters &) const;

    /**
     * Start event-loop in block mode.
     */
    Php::Value run();

    /**
     * Excecute at most one handler within the event loop in block mode.
     */
    Php::Value runOne();

    /**
    * Start event-loop in non-block mode.
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
    Php::Value stopped();

    /**
     * Execute a given callback with argument at the next tick.
     */
    void post(Php::Parameters &);
};
