<?php

$read_cb = function (\Asio\TcpConnection $connection, $data, $length, $ec, $arg) use (&$read_cb) {
    echo 'Client sent: ', $data, PHP_EOL;
    $connection->write('Server received: '.$data.PHP_EOL, false);
    $connection->read(64, true, $read_cb);
};

$service = new Asio\Service;
$service->addTcpServer('0.0.0.0', 8081, function (\Asio\TcpConnection $connection, $ec, $arg) use ($read_cb) {
    $connection->write('Connected to server.'.PHP_EOL, false);
    $connection->read(64, true, $read_cb);
});
$service->run();