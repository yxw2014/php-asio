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

#define PHP_ASIO_ASYNC_HANDLER_SINGLE_ARG std::function<void(const boost::system::error_code&)>( \
    boost::bind(&Future::resolve, future, boost::asio::placeholders::error, 0U))
#define PHP_ASIO_ASYNC_HANDLER_DOUBLE_ARG std::function<void(const boost::system::error_code&, unsigned)>( \
    boost::bind(&Future::resolve, future, boost::asio::placeholders::error, _2))
