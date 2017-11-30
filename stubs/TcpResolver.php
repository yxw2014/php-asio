<?php
/**
 * php-asio/stubs/TcpResolver.php
 *
 * This file is a stub of php-asio, which is implemented in extension asio.so.
 * For source code of asio.so, see src/ directory.
 *
 * @author CismonX<admin@cismon.net>
 */

namespace Asio;

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

    /**
     * {@inheritdoc}
     */
    function cancel() {}
}