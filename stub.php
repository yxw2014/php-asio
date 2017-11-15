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
     * Add Timer.
     *
     * @return Timer
     */
    function addTimer() {}

    /**
     * Add TCP server.
     *
     * @param string $address : Local IP address. IPv4(e.g. '0.0.0.0') and IPv6(e.g. '::') supported.
     * @param int $port : Local port to bind to.
     * @throws \Exception
     * @return TcpServer
     */
    function addTcpServer(string $address, int $port) {}

    /**
     * Add UNIX stream socket server.
     *
     * @param string $path : Path to socket file.
     * @throws \Exception
     * @return UnixServer
     */
    function addUnixServer(string $path) {}

    /**
     * Add signal handler.
     *
     * @throws \Exception
     * @return Signal
     */
    function addSignal() {}

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

    /**
     * Get last error code emitted by yielded async operations of this thread.
     *
     * @return int
     */
    static function lastError() {}
}

/**
 * Returned by asynchronous operations, and resolves upon operation completion.
 * If yielded, future resolution will also resume the generator.
 *
 * @package Asio
 */
final class Future {

    /**
     * This class can only be instantiated by asynchronous operations.
     */
    private function __construct() {}
}

/**
 * Wrapper for Boost.Asio deadline timer.
 * The timer object will be destroyed when timer is no longer deferred.
 *
 * @package Asio
 */
final class Timer {

    /**
     * This class can only be instantiated using "Service::addTimer()".
     */
    private function __construct() {}

    /**
     * Defer the timer once.
     * Do not pass arguments and the original ones will be preserved.
     *
     * @param int $interval : Milliseconds
     * @param callable $callback[optional]
     * @param mixed $argument
     * @throws \Exception
     * @return Future : Resolves null
     */
    function wait(int $interval, callable $callback, $argument = null) {}

    /**
     * Cancel the timer.
     * All async operations will resolve immediately with error.
     */
    function cancel() {}
}

/**
 * Interface Socket
 *
 * @package Asio
 */
interface Socket {

    /**
     * Determine the number of bytes available for reading.
     *
     * @return int
     */
    function available();

    /**
     * Determine whether the socket is at the out-of-band data mark.
     *
     * @return bool
     */
    function atMark();

    /**
     * Close socket.
     *
     * @return void
     */
    function close();
}

/**
 * Interface StreamSocket
 *
 * @package Asio
 */
interface StreamSocket extends Socket {

    /**
     * Asynchronously read from the stream socket.
     *
     * @param int $length : Max number of bytes to be read
     * @param bool $read_some
     * @param callable $callback[optional] : Read handler callback
     * @param mixed $argument
     * @throws \Exception
     * @return Future : Resolves received data(string)
     */
    function read(int $length, bool $read_some, callable $callback, $argument = null);

    /**
     * Asynchronously write to the stream socket.
     *
     * @param string $data : Write buffer
     * @param bool $write_some
     * @param callable $callback[optional] : Write handler callback
     * @param mixed $argument
     * @throws \Exception
     * @return Future : Resolves bytes transferred(int)
     */
    function write(string $data, bool $write_some, callable $callback = null, $argument = null);
}

/**
 * Interface Server
 *
 * @package Asio
 */
interface Server {

    /**
     * Accept incoming client connection once.
     *
     * @param callable $callback[optional] : Acceptor callback
     * @param mixed $argument
     * @throws \Exception
     * @return Future : Resolves Socket.
     */
    function accept(callable $callback, $argument = null);

    /**
     * Stop server and cancel all async operations related to it.
     *
     * @return void
     */
    function stop();
}

/**
 * Wrapper for Boost.Asio TCP acceptor.
 *
 * @package Asio
 */
final class TcpServer implements Server {

    /**
     * This class can only be instantiated using "Service::addTcpServer()".
     */
    private function __construct() {}

    /**
     * {@inheritdoc}
     */
    function accept(callable $callback = null, $argument = null) {}

    /**
     * {@inheritdoc}
     */
    function stop() {}
}

/**
 * Wrapper for Boost.Asio local stream_protocol acceptor.
 *
 * @package Asio
 */
final class UnixServer implements Server {

    /**
     * This class can only be instantiated using "Service::addUnixServer()".
     */
    private function __construct() {}

    /**
     * {@inheritdoc}
     */
    function accept(callable $callback = null, $argument = null) {}

    /**
     * {@inheritdoc}
     */
    function stop() {}
}

/**
 * Wrapper for Boost.Asio TCP socket.
 *
 * @package Asio
 */
final class TcpSocket implements StreamSocket {

    /**
     * This class can only be instantiated by TcpServer.
     */
    private function __construct() {}

    /**
     * {@inheritdoc}
     */
    function read(int $length, bool $read_some, callable $callback = null, $argument = null) {}

    /**
     * {@inheritdoc}
     */
    function write(string $data, bool $write_some, callable $callback = null, $argument = null) {}

    /**
     * {@inheritdoc}
     */
    function available() {}

    /**
     * {@inheritdoc}
     */
    function atMark() {}

    /**
     * {@inheritdoc}
     */
    function close() {}
}

/**
 * Wrapper for Boost.Asio local stream_protocol socket.
 *
 * @package Asio
 */
final class UnixSocket implements StreamSocket {

    /**
     * This class can only be instantiated by UnixServer.
     */
    private function __construct() {}

    /**
     * {@inheritdoc}
     */
    function read(int $length, bool $read_some, callable $callback = null, $argument = null) {}

    /**
     * {@inheritdoc}
     */
    function write(string $data, bool $write_some, callable $callback = null, $argument = null) {}

    /**
     * {@inheritdoc}
     */
    function available() {}

    /**
     * {@inheritdoc}
     */
    function atMark() {}

    /**
     * {@inheritdoc}
     */
    function close() {}
}

/**
 * Wrapper for Boost.Asio signal_set.
 * @package Asio
 */
final class Signal {

    /**
     * This class can only be instantiated using "Service::addSignal()".
     */
    private function __construct() {}

    /**
     * Add signals.
     *
     * @param int[] ...$signals[optional]
     * @throws \Exception
     * @return int : Error code
     */
    function add(int... $signals) {}

    /**
     * Wait for a signal.
     *
     * @param callable $callback[optional]
     * @param mixed $argument
     */
    function wait(callable $callback, $argument = null) {}

    /**
     * Remove signals.
     *
     * @param int[] ...$signals[optional]
     * @throws \Exception
     * @return int : Error code
     */
    function remove(int... $signals) {}

    /**
     * Remove all signals.
     *
     * @throws \Exception
     * @return int : Error code
     */
    function clear() {}

    /**
     * Cancel signal handler.
     *
     * @throws \Exception
     * @return int : Error code
     */
    function cancel() {}
}