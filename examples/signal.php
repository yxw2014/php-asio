<?php
/**
 * Example for php-asio signal handling.
 *
 * @author CismonX<admin@cismon.net>
 */

$service = new Asio\Service;
$signal = $service->addSignal(function (Asio\Signal $signal, int $sig_num, $arg, $ec) {
    //Operation cancelled.
    if ($ec == 125)
        return;
    echo "Server received signal $sig_num. Exiting...\n";
    exit(0);
}, null, SIGINT, SIGTERM);
$service->run();