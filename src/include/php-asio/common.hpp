/**
 * php-asio/include/php-asio/common.hpp
 *
 * @author CismonX<admin@cismon.net>
 */

#pragma once

#include <phpcpp.h>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time.hpp>
#include <boost/filesystem.hpp>

// Handlers with one argument is treated as ones with two arguments.
// No worry about performance. It will be optimized out.
#define NOARG int
#define ASYNC_HANDLER_SINGLE_ARG std::function<void(const boost::system::error_code&)>( \
    boost::bind(&Future::resolve<NOARG>, future, boost::asio::placeholders::error, 0))
#define ASYNC_HANDLER_DOUBLE_ARG(type) std::function<void(const boost::system::error_code&, type)>( \
    boost::bind(&Future::resolve<type>, future, boost::asio::placeholders::error, _2))
