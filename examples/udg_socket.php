<?php
/**
 * Example for php-asio UNIX damain socket (SOCK_DGRAM).
 *
 * @author CismonX<admin@cismon.net>
 */

$service = new Asio\Service;
$service->post(function () use ($service) {
    $socket = $service->addUdgSocket();
    $socket->open();
    $socket->bind('/tmp/test.sock');
    $socket->recvFrom(100,
        function (Asio\UdgSocket $socket, string $data, string $remote_path, int $length, int $ec) {
            if ($ec) {
                echo 'Receive failed. ', posix_strerror($ec), PHP_EOL;
                $socket->close();
                return;
            }
            echo "Remote $remote_path sent \"$data\".", PHP_EOL;
            yield $socket->sendTo('bye', $remote_path);
            $socket->close();
        });
    $socket = $service->addUdgSocket();
    $socket->open();
    //Client socket must explicitly bind to a local endpoint so that remote can respond.
    $socket->bind('/tmp/test2.sock');
    yield $socket->sendTo('hello world', '/tmp/test.sock');
    $response = yield $socket->recvFrom(100);
    echo "Server responded with \"", $response['data'], '".', PHP_EOL;
    $socket->close();
});
$service->run();
