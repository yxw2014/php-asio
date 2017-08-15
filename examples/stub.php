<?php

/**
 * Class Asio
 *
 * This file is a stub of class Asio, which is implemented in extension asio.so.
 * For source code of asio.so, see src/ directory.
 */
class Asio {

    /**
     * Add Timer.
     *
     * @param int $interval : milliseconds
     * @param callable $callback
     * @param mixed $argument
     * @param bool $persistent
     * @return int : timer_id
     */
    function addTimer(int $interval, callable $callback, $argument = null, bool $persistent = true) {}

    /**
     * Delete timer.
     *
     * @param int $timer_id
     */
    function delTimer(int $timer_id) {}

    /**
     * Run event loop(blocked).
     *
     * @return int : the number of handlers executed
     */
    function run() {}

    /**
     * Execute at most one handler in event loop(blocked).
     *
     * @return int : the number of handlers executed
     */
    function runOne() {}

    /**
     * Run event loop(not blocked).
     *
     * @return int : the number of handlers executed
     */
    function poll() {}

    /**
     * Execute at most one handler in event loop(not blocked).
     *
     * @return int : the number of handlers executed
     */
    function pollOne() {}

    /**
     * Execute the given callback the next tick.
     *
     * @param callable $callback
     * @param mixed $argument
     * @return int : the number of handlers executed
     */
    function post(callable $callback, $argument = null) {}
}