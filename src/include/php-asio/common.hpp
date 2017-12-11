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
#define ENABLE_COROUTINE
#ifdef ENABLE_COROUTINE
#define CORO_RETVAL Php::Value
#define CORO_RETURN(value) return value
#define CORO_REGISTER(value) Future::coroutine(value)
#define FUTURE_RETURN return
#else
#define CORO_RETVAL void
#define CORO_RETURN(value)
#define CORO_REGISTER(value) value
#define FUTURE_RETURN
#endif // ENABLE_COROUTINE
#define FUTURE_RETVAL CORO_RETVAL

// If you don't need multi-threading support for I/O objects, you can disable Strand for better performance.
#define ENABLE_STRAND
#ifdef ENABLE_STRAND
#define STRAND_UNWRAP future->strand(callback)
#define STRAND_RESOLVE(arg) future->get_strand() ? future->get_strand()->wrap(arg) : arg
#else
#define STRAND_UNWRAP callback
#define STRAND_RESOLVE(arg) arg
#endif // ENABLE_STRAND

using boost::asio::ip::tcp;
using boost::asio::ip::udp;
using unix = boost::asio::local::stream_protocol;
using udg = boost::asio::local::datagram_protocol;
