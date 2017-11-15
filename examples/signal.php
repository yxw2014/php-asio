<?php
/**
 * Example for php-asio signal handling.
 *
 * @author CismonX<admin@cismon.net>
 */

$service = new Asio\Service;
$signal = $service->addSignal();
$signal->add(SIGINT, SIGTERM);
$signal->wait(function (Asio\Signal $signal, int $sig_num, $arg, $ec) {
    if ($ec)
        return;
    echo "Server received signal $sig_num. Exiting...\n";
    exit(0);
}, null);
$service->run();