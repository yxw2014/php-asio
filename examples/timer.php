<?php

$io_service = new Asio;
$io_service->addTimer(1000, function ($timer_id, $arg) use ($io_service) {
    static $counter = 0;
    $io_service->post(function () {
        echo 'Wait for one second...', PHP_EOL;
    });
    echo $arg, PHP_EOL;
    if (++$counter >= 5)
        $io_service->delTimer($timer_id);
}, 'Tick.');
$io_service->run();