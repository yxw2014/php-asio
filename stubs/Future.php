<?php
/**
 * php-asio/stubs/Future.php
 *
 * This file is a stub of php-asio, which is implemented in extension asio.so.
 * For source code of asio.so, see src/ directory.
 *
 * @author CismonX<admin@cismon.net>
 */

namespace Asio;

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