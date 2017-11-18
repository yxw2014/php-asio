/**
 * php-asio/service.cpp
 *
 * @author CismonX<admin@cismon.net>
 */

#include "service.hpp"
#include "timer.hpp"
#include "server.hpp"
#include "signal.hpp"

namespace Asio
{
    Php::Value Service::add_timer()
    {
        return new Timer(io_service_);
    }

    Php::Value Service::add_tcp_server(Php::Parameters& params)
    {
        auto server = new TcpServer(io_service_);
        server->init_acceptor(params[0].stringValue(), static_cast<unsigned short>(params[1].numericValue()));
        return server;
    }

    Php::Value Service::add_unix_server(Php::Parameters& params)
    {
        auto server = new UnixServer(io_service_);
        server->init_acceptor(params[0].stringValue());
        return server;
    }

    Php::Value Service::add_signal()
    {
        return new Signal(io_service_);
    }

    Php::Value Service::run(Php::Parameters& params)
    {
        boost::system::error_code ec;
        auto handler_count = io_service_.run(ec);
        if (params.size())
            params[0] = static_cast<int64_t>(ec.value());
        return static_cast<int64_t>(handler_count);
    }

    Php::Value Service::run_one(Php::Parameters& params)
    {
        boost::system::error_code ec;
        auto handler_count = io_service_.run_one(ec);
        if (params.size())
            params[0] = static_cast<int64_t>(ec.value());
        return static_cast<int64_t>(handler_count);
    }

    Php::Value Service::poll(Php::Parameters& params)
    {
        boost::system::error_code ec;
        auto handler_count = io_service_.poll(ec);
        if (params.size())
            params[0] = static_cast<int64_t>(ec.value());
        return static_cast<int64_t>(handler_count);
    }

    Php::Value Service::poll_one(Php::Parameters& params)
    {
        boost::system::error_code ec;
        auto handler_count = io_service_.poll_one(ec);
        if (params.size())
            params[0] = static_cast<int64_t>(ec.value());
        return static_cast<int64_t>(handler_count);
    }

    void Service::stop()
    {
        io_service_.stop();
    }

    void Service::reset()
    {
        io_service_.reset();
    }

    Php::Value Service::stopped() const
    {
        return io_service_.stopped();
    }

    void Service::post(Php::Parameters& params)
    {
        auto callback = params[0];
        auto argument = params.size() == 1 ? Php::Value() : params[1];
        io_service_.post([callback, argument]()
        {
            Future::coroutine(callback(argument));
        });
    }

    boost::asio::io_service& Service::get_io_service()
    {
        return io_service_;
    }
}
