<?php
/**
 * Example for php-asio TCP server.
 *
 * @author CismonX<admin@cismon.net>
 */

$read_cb = function (\Asio\TcpConnection $connection, $data, $length, $ec, $arg) use (&$read_cb) {
    echo 'Client sent: ', $data, PHP_EOL;
    $connection->write('Server received: '.$data.PHP_EOL, false);
    $connection->read(64, true, $read_cb);
};

$service = new Asio\Service;
//Unlike that of PHP (`stream_socket_server()`), IPv6 addresses should not be put between a pair of brackets.
$service->addTcpServer('::', 8081, function (\Asio\TcpConnection $connection, $ec, $arg) use ($read_cb) {
    $connection->write('Connected to server.'.PHP_EOL, false);
    $connection->read(64, true, $read_cb);
});
$service->run();