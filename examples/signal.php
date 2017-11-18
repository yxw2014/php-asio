<?php
/**
 * Example for php-asio signal handling.
 *
 * @author CismonX<admin@cismon.net>
 */

$service = new Asio\Service;
// Using Service::post.
$service->post(function () use ($service) {
    $signal = $service->addSignal();
    $signal->add(SIGINT, SIGTERM);
    $sig_num = yield $signal->wait();
    if (Asio\lastError()) {
        $signal->cancel();
        return;
    }
    echo "Server received signal $sig_num. Send signal again to exit.\n";
    yield $signal->wait();
});
//Service stop running when there are no pending async operations.
$service->run();
