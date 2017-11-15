<?php
/**
 * Example for php-asio timer.
 *
 * @author CismonX<admin@cismon.net>
 */

$service = new Asio\Service();
$service->addTimer()->wait(1000, function (Asio\Timer $timer, $arg, $ec) use ($service) {
    if ($ec)
        return;
    $counter = 0;
    while (1) {
        $service->post(function () {
            echo 'Wait for one second...', PHP_EOL;
        });
        echo $arg, PHP_EOL;
        if (++$counter >= 5) {
            $timer->cancel();
            return;
        } else {
            //Using coroutine.
            yield $timer->wait(1000);
            if (\Asio\Service::lastError())
                return;
        }
    }
}, 'Tick.');
$service->run();