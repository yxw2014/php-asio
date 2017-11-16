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
        auto port = params[1].numericValue();
        if (port < 0 || port > 65535)
            throw Php::Exception("Bad port number.");
        auto server = new TcpServer(io_service_);
        server->init_acceptor(params[0].stringValue(), static_cast<unsigned short>(port));
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

    Php::Value Service::run()
    {
        return static_cast<int64_t>(io_service_.run());
    }

    Php::Value Service::run_one()
    {
        return static_cast<int64_t>(io_service_.run_one());
    }

    Php::Value Service::poll()
    {
        return static_cast<int64_t>(io_service_.poll());
    }

    Php::Value Service::poll_one()
    {
        return static_cast<int64_t>(io_service_.poll_one());
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
        if (!callback.isCallable())
            throw Php::Exception("Handler not callable.");
        auto argument = params.size() == 1 ? Php::Value() : params[1];
        io_service_.post([callback, argument]()
        {
            Future::coroutine(callback(argument));
        });
    }

    Php::Value Service::get_last_error()
    {
        return last_error_;
    }

    thread_local int64_t Service::last_error_ = 0;
}