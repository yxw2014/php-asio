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
    while (1) {
        echo 'Client sent: ', $data, PHP_EOL;
        //Wait for write completion before read again.
        $bytes_transferred = yield $socket->write('Server received: '.$data.PHP_EOL, false);
        //Socket read resolves received data.
        $data = yield $socket->read(64, true);
        if (\Asio\Service::lastError()) {
            $socket->close();
            return;
        }
    }
};
$acceptor_cb = function (Asio\Server $server, Asio\StreamSocket $socket, $ec, $arg) use ($read_cb, &$acceptor_cb) {
    if ($ec) {
        $server->stop();
        return;
    }
    while (1) {
        $socket->write('Connected to server.'.PHP_EOL, false);
        $socket->read(64, true, $read_cb);
        //Server accept resolves Socket.
        $socket = yield $server->accept();
        if (\Asio\Service::lastError()) {
            $server->stop();
            return;
        }
    }
};

$service = new Asio\Service;
//Unlike that of PHP (`stream_socket_server()`), IPv6 addresses should not be put between a pair of brackets.
$service->addTcpServer('::', 8081)->accept($acceptor_cb);
$service->addUnixServer('/tmp/test.sock')->accept($acceptor_cb);
$service->run();