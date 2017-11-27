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
     * Add a new timer.
     *
     * @return Timer
     */
    function addTimer() {}

    /**
     * Add a new TCP acceptor.
     *
     * @return TcpAcceptor
     */
    function addTcpAcceptor() {}

    /**
     * Add a new UNIX domain socket acceptor (SOCK_STREAM).
     *
     * @return UnixAcceptor
     */
    function addUnixAcceptor() {}

    /**
     * Add a TCP resolver.
     *
     * @return TcpResolver
     */
    function addTcpResolver() {}

    /**
     * Add a UDP resolver.
     *
     * @return UdpResolver
     */
    function addUdpResolver() {}

    /**
     * Add a new TCP socket.
     *
     * @return TcpSocket
     */
    function addTcpSocket() {}

    /**
     * Add a new UNIX domain socket (SOCK_STREAM).
     *
     * @return UnixSocket
     */
    function addUnixSocket() {}

    /**
     * Add new signal set.
     *
     * @throws \Exception
     * @return Signal
     */
    function addSignal() {}

    /**
     * Run the event loop until stopped or no more work.
     *
     * @param int $ec[optional] : Error code
     * @return int : the number of handlers executed
     */
    function run(int &$ec) {}

    /**
     * Run until stopped or one operation is performed.
     *
     * @param int $ec[optional] : Error code
     * @return int : the number of handlers executed
     */
    function runOne(int &$ec) {}

    /**
     * Poll for operations without blocking.
     *
     * @param int $ec[optional] : Error code
     * @return int : the number of handlers executed
     */
    function poll(int &$ec) {}

    /**
     * Poll for one operation without blocking.
     *
     * @param int $ec[optional] : Error code
     * @return int : the number of handlers executed
     */
    function pollOne(int &$ec) {}

    /**
     * Request invocation of the given handler and return immediately.
     *
     * @param callable $callback
     * @param mixed $argument
     * @return int : the number of handlers executed
     */
    function post(callable $callback, $argument = null) {}

    /**
     * Request invocation of the given handler.
     *
     * @param callable $callback
     * @param mixed $argument
     * @return int : the number of handlers executed
     */
    function dispatch(callable $callback, $argument = null) {}

    /**
     * Notify all services of a fork event.
     *
     * @param bool $is_parent[optional] : Empty for `fork_prepare`
     * @throws \Error : Throws fatal error when an error occurs
     * @return void
     */
    function notify_fork(bool $is_parent) {}

    /**
     * Stop the event processing loop.
     *
     * @return void
     */
    function stop() {}

    /**
     * Reset in preparation for a subsequent run invocation.
     *
     * @return void
     */
    function reset() {}

    /**
     * Determine whether the io_service is stopped.
     */
    function stopped() {}
}

/**
 * When an asynchronous operation completes, its Future will be resolved.
 * And the corresponding coroutine will resume (if Future was yielded by a Generator).
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
     * Read asynchronously from stream socket.
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
     * Write asynchronously to stream socket.
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
     * Bind socket to a local endpoint.
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
     * Bind socket to a local endpoint.
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
     * Put the acceptor into the state where it may accept new connections.
     *
     * @param int $backlog[optional] : The maximum length of the queue of pending connections.
     *                                 Default to `boost::asio::socket_base::max_connections`.
     * @return int : Error code
     */
    function listen(int $backlog);

    /**
     * Asynchronously accept a new connection into a socket.
     *
     * @param callable $callback[optional] : Acceptor callback
     * @param mixed $argument
     * @return Future : Resolves Socket.
     */
    function accept(callable $callback, $argument = null);

    /**
     * Cancel async operations and stop acceptor.
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
     * Initiate an asynchronous resolve against the resolver.
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
     * This class can only be instantiated using `Service::addTcpAcceptor()`.
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
     * This class can only be instantiated using `Service::addUnixAcceptor()`.
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
     * This class can only be instantiated using `Service::addTcpSocket()` and `TcpAcceptor::Accept()`.
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
     * This class can only be instantiated using `Service::addUnixSocket()` and `UnixAcceptor::Accept()`.
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
     * This class can only be instantiated using `Service::addTcpResolver()`.
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
     * This class can only be instantiated using `Service::addTcpResolver()`.
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
     * This class can only be instantiated using `Service::addSignal()`.
     */
    private function __construct() {}

    /**
     * Add the specified signal(s) to the signal set.
     *
     * @param int[] ...$signals[optional]
     * @throws \Exception
     * @return int : Error code
     */
    function add(int... $signals) {}

    /**
     * Initiate an asynchronous wait against the signal set.
     *
     * @param callable $callback[optional]
     * @param mixed $argument
     * @return Future
     */
    function wait(callable $callback, $argument = null) {}

    /**
     * Remove the specified signal(s) from the signal set.
     *
     * @param int[] ...$signals[optional]
     * @throws \Exception
     * @return int : Error code
     */
    function remove(int... $signals) {}

    /**
     * Remove all signals from the signal set.
     *
     * @throws \Exception
     * @return int : Error code
     */
    function clear() {}

    /**
     * Cancel current signal set.
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
