<?php
/**
 * Example for php-asio UNIX domain socket client.
 *
 * @author CismonX<admin@cismon.net>
 */

$service = new Asio\Service;
$service->post(function () use ($service) {
    $socket = $service->addUnixSocket();
    $socket->open();
    yield $socket->connect('/tmp/test.sock');
    if ($ec = Asio\lastError()) {
        echo 'Connect failed. Error code: ', $ec, PHP_EOL;
        $socket->close();
        return;
    }
    $data = yield $socket->read(100);
    if ($ec = Asio\lastError()) {
        echo 'Read failed. Error code: ', $ec, PHP_EOL;
        $socket->close();
        return;
    }
    echo "Client received: \"$data\"", PHP_EOL;
    yield $socket->write('hello world');
    if ($ec = Asio\lastError()) {
        echo 'Write failed. Error code: ', $ec, PHP_EOL;
        $socket->close();
        return;
    }
});
$service->run();
