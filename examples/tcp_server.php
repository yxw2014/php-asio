<?php
/**
 * Example for php-asio TCP server.
 *
 * @author CismonX<admin@cismon.net>
 */

$read_cb = function (Asio\TcpSocket $socket, $data, $length, $ec, $arg) use (&$read_cb) {
    //Operation cancelled.
    if ($ec == 125)
        return;
    echo 'Client sent: ', $data, PHP_EOL;
    $socket->write('Server received: '.$data.PHP_EOL, false);
    $socket->read(64, true, $read_cb);
};

$service = new Asio\Service;
//Unlike that of PHP (`stream_socket_server()`), IPv6 addresses should not be put between a pair of brackets.
$service->addTcpServer('::', 8081,
    function (Asio\TcpServer $server, Asio\TcpSocket $socket, $ec, $arg) use ($read_cb) {
        //Operation cancelled.
        if ($ec == 125)
            return;
        $socket->write('Connected to server.'.PHP_EOL, false);
        $socket->read(64, true, $read_cb);
    }
);
$service->run();