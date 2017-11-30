<?php
/**
 * Example for php-asio stream socket server.
 *
 * @author CismonX<admin@cismon.net>
 */

$service = new Asio\Service;
$acceptor = $service->addTcpAcceptor();
$acceptor->open(true);
// Unlike that of PHP (`stream_socket_server()`), IPv6 addresses should not be put between a pair of brackets.
if ($ec = $acceptor->bind('::', 8081)) {
    echo 'Bind failed. ', posix_strerror($ec), PHP_EOL;
    return;
}
$acceptor->listen(20);
$acceptor->accept(
    $acceptor_cb = function (Asio\Acceptor $server, Asio\StreamSocket $socket, $ec) use (&$acceptor_cb) {
        if ($ec) {
            echo 'Accept failed. ', posix_strerror($ec), PHP_EOL;
            return;
        }
        if ($socket instanceof Asio\TcpSocket)
            echo 'Remote ', $socket->remoteAddr(), ':', $socket->remotePort(), ' Connected.', PHP_EOL;
        elseif ($socket instanceof Asio\UnixSocket)
            echo 'Remote ', $socket->remotePath(), ' Connected.', PHP_EOL;
        $server->accept($acceptor_cb);
        yield $socket->write('Connected to server.'.PHP_EOL, false);
        $socket->read(64, true,
            $read_cb = function (Asio\StreamSocket $socket, $data, $length, $ec) use (&$read_cb) {
                if ($ec) {
                    echo 'Read fail. ', posix_strerror($ec), PHP_EOL;
                    $socket->close();
                    return;
                }
                echo 'Client sent: ', trim($data), PHP_EOL;
                //Socket write resolves number of bytes transferred.
                $bytes_transferred = yield $socket->write('Server received: ' . trim($data) . PHP_EOL, false);
                if ($ec = Asio\Service::lastError()) {
                    echo 'Write fail. ', posix_strerror($ec), PHP_EOL;
                    $socket->close();
                    return;
                }
                echo $bytes_transferred, ' bytes written.', PHP_EOL;
                $socket->read(64, true, $read_cb);
            });
    });
$acceptor = $service->addUnixAcceptor();
$acceptor->open();
if ($ec = $acceptor->bind('/tmp/test.sock')) {
    echo 'Bind failed. ', posix_strerror($ec), PHP_EOL;
    return;
}
$acceptor->listen(20);
$acceptor->accept($acceptor_cb);
$service->run();
