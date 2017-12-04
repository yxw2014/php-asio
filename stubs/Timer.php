<?php
/**
 * php-asio/stubs/Timer.php
 *
 * This file is a stub of php-asio, which is implemented in extension asio.so.
 * For source code of asio.so, see src/ directory.
 *
 * @author CismonX<admin@cismon.net>
 */

namespace Asio;

/**
 * Wrapper for Boost.Asio deadline_timer.
 *
 * @package Asio
 */
final class Timer
{
    /**
     * This class can only be instantiated using "Service::addTimer()".
     */
    private function __construct() {}

    /**
     * Set timer expiry time.
     *
     * @param int $time : Milliseconds from now or UNIX timestamp(in milliseconds).
     * @param bool $use_timestamp : Use timestamp or relative time as first parameter.
     * @return int : Error code.
     */
    function expire(int $time, bool $use_timestamp = false) {}

    /**
     * Initiate an asynchronous wait against the timer.
     *
     * @param callable $callback[optional]
     * @param mixed $argument
     * @throws \Exception
     * @return Future : Resolves null
     */
    function wait(callable $callback, $argument = null) {}

    /**
     * Cancel the timer.
     * All async operations will resolve immediately with error.
     *
     * @return int : Error code
     */
    function cancel() {}
}