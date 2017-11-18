<?php
/**
 * Example for php-asio timer.
 *
 * @author CismonX<admin@cismon.net>
 */

$service = new Asio\Service();
$timer = $service->addTimer();
$timer->expire(1000);
$timer->wait(function (Asio\Timer $timer, $arg, $ec) use ($service) {
    if ($ec)
        return;
    $counter = 0;
    while (1) {
        // Using extra argument.
        echo $arg, PHP_EOL;
        if (++$counter >= 5) {
            $timer->cancel();
            return;
        } else {
            // Using timestamp.
            $timer->expire(round(1000 * (microtime(true) + 1)), true);
            // Using coroutine.
            yield $timer->wait();
            if (Asio\lastError())
                return;
        }
    }
}, 'Tick.');
$service->run();
