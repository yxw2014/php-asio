#pragma once

#include <phpcpp.h>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time.hpp>

// #define PHP_ASIO_MT;

// Uncomment above if multithreading is required.
// In this case, you must link Boost libraries with -mt suffix.
// And PHP must be compile with ZTS enabled.

#include "counter.hpp"
#include "io_service.hpp"
#include "timer.hpp"
#include "asio.hpp"