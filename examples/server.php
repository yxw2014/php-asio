<?php
/**
 * Example for php-asio stream socket server.
 *
 * @author CismonX<admin@cismon.net>
 */

$read_cb = function (Asio\StreamSocket $socket, $data, $length, $ec, $arg) use (&$read_cb) {
    if ($ec) {
        $socket->close();
        return;
    }
    echo 'Client sent: ', $data, PHP_EOL;
    $socket->write('Server received: '.$data.PHP_EOL, false);
    $socket->read(64, true, $read_cb);
};
$acceptor_cb = function (Asio\Server $server, Asio\StreamSocket $socket, $ec, $arg) use ($read_cb) {
    if ($ec) {
        $server->stop();
        return;
    }
    $socket->write('Connected to server.'.PHP_EOL, false);
    $socket->read(64, true, $read_cb);
};

$service = new Asio\Service;
//Unlike that of PHP (`stream_socket_server()`), IPv6 addresses should not be put between a pair of brackets.
$service->addTcpServer('::', 8081, $acceptor_cb);
$service->addUnixServer('/tmp/test.sock', $acceptor_cb);
$service->run();