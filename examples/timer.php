<?php

$service = new Asio\Service();
$timer = $service->addTimer(1000, function (Asio\Timer $timer, $arg, $ec) use ($service) {
    static $counter = 0;
    $service->post(function () {
        echo 'Wait for one second...', PHP_EOL;
    });
    echo $arg, PHP_EOL;
    if (++$counter >= 5)
        $timer->cancel();
}, 'Tick.');
$service->run();