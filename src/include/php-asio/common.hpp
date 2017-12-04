/**
 * php-asio/include/common.hpp
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
#define NOARG int
#define ASYNC_HANDLER_SINGLE_ARG std::function<void(const boost::system::error_code&)>( \
    boost::bind(&Future::resolve<NOARG>, future, boost::asio::placeholders::error, 0))
#define ASYNC_HANDLER_DOUBLE_ARG(type) std::function<void(const boost::system::error_code&, type)>( \
    boost::bind(&Future::resolve<type>, future, boost::asio::placeholders::error, _2))

// If you don't need coroutines, you can turn it off for better performance.
#define ENABLE_COROUTINE true
#if ENABLE_COROUTINE
#define CORO_RETVAL Php::Value
#define CORO_RETURN(value) return value
#define CORO_REGISTER(value) Future::coroutine(value)
#define FUTURE_RETURN return
#else
#define CORO_RETVAL void
#define CORO_RETURN(value)
#define CORO_REGISTER(value) value
#define FUTURE_RETURN
#endif
#define FUTURE_RETVAL CORO_RETVAL

using boost::asio::ip::tcp;
using boost::asio::ip::udp;
using unix = boost::asio::local::stream_protocol;
using udg = boost::asio::local::datagram_protocol;
