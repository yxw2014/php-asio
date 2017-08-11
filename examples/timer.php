<?php

$io_service = new Asio;
$io_service->addTimer(1000, function ($str) use ($io_service) {
    $io_service->post(function () {
        echo 'Wait for one second...', PHP_EOL;
    });
    echo $str, PHP_EOL;
}, 'Tick.');
$io_service->run();