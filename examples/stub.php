<?php

/**
 * This file is a stub of php-asio, which is implemented in extension asio.so.
 * For source code of asio.so, see src/ directory.
 *
 * @author CismonX<admin@cismon.net>
 */
namespace Asio;

/**
 * Wrapper for Boost.Asio IO service.
 * Provide access to instantiation of IO objects.
 *
 * @package Asio
 */
class Service {

    /**
     * Add Timer and start deferring.
     *
     * @param int $interval : milliseconds
     * @param callable $callback : Timer callback
     * @param mixed $argument : Extra argument
     * @param bool $persistent
     * @return Timer
     */
    function addTimer(int $interval, callable $callback, $argument = null, bool $persistent = true) {}

    /**
     * @param string $address : Local IP address. IPv4(e.g. '0.0.0.0') and IPv6(e.g. '::') supported.
     * @param int $port : Local port to bind to.
     * @param callable $callback[optional] : Acceptor callback
     * @param mixed $argument
     * @return TcpConnection
     */
    function addTcpServer(string $address, int $port, callable $callback, $argument = null) {}

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

/**
 * Wrapper for Boost.Asio deadline timer.
 * The timer object will be destroyed when timer is no longer deferred.
 *
 * @package Asio
 */
class Timer {

    /**
     * This class can only be instantiated using "Service::addTimer()".
     */
    private function __construct() {}

    /**
     * Defer the timer once.
     * Do not pass arguments and the original ones will be preserved.
     *
     * This method only work for timers with "persistent == false".
     * And it cannot be called outside timer callback.
     *
     * @param int $interval[optional]
     * @param callable $callback[optional]
     * @param mixed $argument
     */
    function defer(int $interval, callable $callback, $argument = null) {}

    /**
     * Set timer's persistent attribute to false.
     * If this method is called outside timer callback, the callback will still be called once.
     */
    function cancel() {}
}

/**
 * Wrapper for Boost.Asio TCP acceptor.
 *
 * @package Asio
 */
class TcpServer {

    /**
     * This class can only be instantiated using "Service::addTcpServer()".
     */
    private function __construct() {}

    /**
     * Accept incoming client connection once.
     *
     * @param callable $callback : Acceptor callback
     * @param mixed $argument
     */
    function accept(callable $callback, $argument = null) {}

    /**
     * Stop TCP server and cancel all async operations related to it.
     */
    function stop() {}
}

class TcpConnection {

    /**
     * This class can only be instantiated by TcpServer and TcpClient.
     */
    private function __construct() {}

    /**
     * Asynchronously read from the stream socket of this connection.
     *
     * @param int $length : Max number of bytes to be read
     * @param bool $read_some
     * @param callable $callback : Read handler callback
     * @param mixed $argument
     */
    function read(int $length, bool $read_some, callable $callback, $argument = null) {}

    /**
     * Asynchronously write to the stream socket of this connection.
     *
     * @param string $data : Write buffer
     * @param bool $write_some
     * @param callable $callback[optional] : Write handler callback
     * @param null $argument
     */
    function write(string $data, bool $write_some, callable $callback, $argument = null) {}

    /**
     * Determine the number of bytes available for reading.
     *
     * @return int
     */
    function available() {}

    /**
     * Determine whether the socket is at the out-of-band data mark.
     *
     * @return bool
     */
    function atMark() {}

    /**
     * Close TCP connection.
     */
    function close() {}
}