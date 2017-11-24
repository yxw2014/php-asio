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
class Service
{
    /**
     * Add Timer.
     *
     * @return Timer
     */
    function addTimer() {}

    /**
     * Add TCP acceptor.
     *
     * @return TcpAcceptor
     */
    function addTcpAcceptor() {}

    /**
     * Add UNIX stream socket acceptor.
     *
     * @return UnixAcceptor
     */
    function addUnixAcceptor() {}

    /**
     * Resolve TCP-based services.
     *
     * @return TcpResolver
     */
    function addTcpResolver() {}

    /**
     * Resolve UDP-based services.
     *
     * @return UdpResolver
     */
    function addUdpResolver() {}

    /**
     * Add a TCP socket.
     *
     * @return TcpSocket
     */
    function addTcpSocket() {}

    /**
     * Add a UNIX domain socket (SOCK_STREAM).
     *
     * @return UnixSocket
     */
    function addUnixSocket() {}

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
     * @param int $ec[optional] : Error code
     * @return int : the number of handlers executed
     */
    function run(int &$ec) {}

    /**
     * Execute at most one handler in event loop(blocked).
     *
     * @param int $ec[optional] : Error code
     * @return int : the number of handlers executed
     */
    function runOne(int &$ec) {}

    /**
     * Run event loop(not blocked).
     *
     * @param int $ec[optional] : Error code
     * @return int : the number of handlers executed
     */
    function poll(int &$ec) {}

    /**
     * Execute at most one handler in event loop(not blocked).
     *
     * @param int $ec[optional] : Error code
     * @return int : the number of handlers executed
     */
    function pollOne(int &$ec) {}

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
 * Returned by asynchronous operations, and resolves upon operation completion.
 * If yielded, future resolution will also resume the generator.
 *
 * @package Asio
 */
final class Future
{
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
     * Wait for timer to expire.
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

/**
 * Interface Socket
 *
 * @package Asio
 */
interface Socket
{
    /**
     * Determine the number of bytes available for reading.
     *
     * @param int $ec[optional] : Error code
     * @return int
     */
    function available(int &$ec);

    /**
     * Determine whether the socket is at the out-of-band data mark.
     *
     * @param int $ec[optional] : Error code
     * @return bool
     */
    function atMark(int &$ec);

    /**
     * Close socket.
     *
     * @return int : Error code
     */
    function close();
}

/**
 * Interface StreamSocket
 *
 * @package Asio
 */
interface StreamSocket extends Socket
{
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
    function read(int $length, bool $read_some = true, callable $callback, $argument = null);

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
    function write(string $data, bool $write_some = false, callable $callback, $argument = null);
}

/**
 * Interface InetSocket
 *
 * @package Asio
 */
interface InetSocket extends Socket
{
    /**
     * Open socket.
     *
     * @param bool $use_ipv6 : True for AF_INET6, false for AF_INET
     * @return int : Error code
     */
    function open(bool $use_ipv6);

    /**
     * Bind socket to an endpoint.
     *
     * @param string $address
     * @param int $port
     * @return int : Error code
     */
    function bind(string $address, int $port);

    /**
     * Get remote endpoint address.
     *
     * @return string
     */
    function remoteAddr();

    /**
     * Get remote endpoint port.
     *
     * @return mixed
     */
    function remotePort();
}

/**
 * Interface LocalSocket
 *
 * @package Asio
 */
interface LocalSocket extends Socket
{
    /**
     * Open socket.
     *
     * @return int : Error code
     */
    function open();

    /**
     * Bind socket to an endpoint.
     *
     * @param string $path : Path to socket file
     * @return mixed
     */
    function bind(string $path);

    /**
     * Get remote endpoint path.
     *
     * @return string
     */
    function remotePath();
}

/**
 * Interface Server
 *
 * @package Asio
 */
interface Acceptor
{
    /**
     * @param int $backlog[optional] : The maximum length of the queue of pending connections.
     *                                 Default to boost::asio::socket_base::max_connections.
     * @return mixed
     */
    function listen(int $backlog);

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
     * @return int : Error code
     */
    function stop();
}

/**
 * Interface Resolver
 *
 * @package Asio
 */
interface Resolver
{
    /**
     * Start async resolve.
     *
     * @param string $host : Host name
     * @param string $service
     * @param callable $callback[optional] : Resolver callback
     * @param mixed $argument
     * @return Future
     */
    function resolve(string $host, string $service = '', callable $callback, $argument = null);
}

/**
 * Wrapper for Boost.Asio TCP acceptor.
 *
 * @package Asio
 */
final class TcpAcceptor implements Acceptor
{
    /**
     * This class can only be instantiated using "Service::addTcpServer()".
     */
    private function __construct() {}

    /**
     * Open acceptor.
     *
     * @param bool $use_ipv6 : True for AF_INET6, false for AF_INET
     * @return int : Error code
     */
    function open(bool $use_ipv6) {}

    /**
     * Bind acceptor to local endpoint.
     *
     * @param string $address
     * @param int $port
     * @return int : Error code
     */
    function bind(string $address, int $port) {}

    /**
     * {@inheritdoc}
     */
    function listen(int $backlog = null) {}

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
final class UnixAcceptor implements Acceptor
{
    /**
     * This class can only be instantiated using "Service::addUnixServer()".
     */
    private function __construct() {}

    /**
     * Open acceptor.
     *
     * @return int : Error code.
     */
    function open() {}

    /**
     * Bind acceptor to local endpoint.
     *
     * @param string $path
     * @return int : Error code.
     */
    function bind(string $path) {}

    /**
     * {@inheritdoc}
     */
    function listen(int $backlog = null) {}

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
final class TcpSocket implements StreamSocket, InetSocket
{
    /**
     * This class can only be instantiated by TcpServer.
     */
    private function __construct() {}

    /**
     * {@inheritdoc}
     */
    function open(bool $use_ipv6) {}

    /**
     * {@inheritdoc}
     */
    function bind(string $address, int $port) {}

    /**
     * Connect to a remote endpoint.
     *
     * @param string $address : Remote address
     * @param int $port : Remote port
     * @param callable $callback[optional]
     * @param mixed $argument
     */
    function connect(string $address, int $port, callable $callback, $argument = null) {}

    /**
     * {@inheritdoc}
     */
    function read(int $length, bool $read_some = true, callable $callback = null, $argument = null) {}

    /**
     * {@inheritdoc}
     */
    function write(string $data, bool $write_some = false, callable $callback = null, $argument = null) {}

    /**
     * {@inheritdoc}
     */
    function remoteAddr() {}

    /**
     * {@inheritdoc}
     */
    function remotePort() {}

    /**
     * {@inheritdoc}
     */
    function available(int &$ec = null) {}

    /**
     * {@inheritdoc}
     */
    function atMark(int &$ec = null) {}

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
final class UnixSocket implements StreamSocket, LocalSocket
{
    /**
     * This class can only be instantiated by UnixServer.
     */
    private function __construct() {}

    /**
     * {@inheritdoc}
     */
    function open() {}

    /**
     * {@inheritdoc}
     */
    function bind(string $path) {}

    /**
     * Connect to a remote endpoint.
     *
     * @param string $path : Remote socket file path
     * @param callable $callback[optional]
     * @param mixed $argument
     * @return int : Error code
     */
    function connect(string $path, callable $callback, $argument = null) {}

    /**
     * {@inheritdoc}
     */
    function read(int $length, bool $read_some = true, callable $callback = null, $argument = null) {}

    /**
     * {@inheritdoc}
     */
    function write(string $data, bool $write_some = false, callable $callback = null, $argument = null) {}

    /**
     * {@inheritdoc}
     */
    function remotePath() {}

    /**
     * {@inheritdoc}
     */
    function available(int &$ec = null) {}

    /**
     * {@inheritdoc}
     */
    function atMark(int &$ec = null) {}

    /**
     * {@inheritdoc}
     */
    function close() {}
}

/**
 * Wrapper for Boost.Asio TCP resolver.
 *
 * @package Asio
 */
final class TcpResolver implements Resolver
{
    /**
     * This class can only be instantiated using "Service::addTcpResolver()".
     */
    private function __construct() {}

    /**
     * {@inheritdoc}
     */
    function resolve(string $host, string $service = '', callable $callback = null, $argument = null) {}
}

/**
 * Wrapper for Boost.Asio UDP resolver.
 *
 * @package Asio
 */
final class UdpResolver implements Resolver
{
    /**
     * This class can only be instantiated using "Service::addTcpResolver()".
     */
    private function __construct() {}

    /**
     * {@inheritdoc}
     */
    function resolve(string $host, string $service = '', callable $callback = null, $argument = null) {}
}

/**
 * Wrapper for Boost.Asio signal_set.
 *
 * @package Asio
 */
final class Signal
{
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
     * @return Future
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

/**
 * Get last error code emitted by yielded async operations of this thread.
 *
 * @return int
 */
function lastError() {}
