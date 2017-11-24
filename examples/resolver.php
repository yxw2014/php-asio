<?php
/**
 * Example for php-asio Resolver.
 *
 * @author CismonX<admin@cismon.net>
 */

$service = new Asio\Service;
$service->post(function () use ($service) {
    $resolver = $service->addTcpResolver();
    $data = yield $resolver->resolve('github.com', 'http');
    foreach ($data as $addr)
        echo $addr, PHP_EOL;
});
$service->run();
