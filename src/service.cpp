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
    Php::Value Service::add_timer(Php::Parameters& params)
    {
        auto callback = params[1];
        auto param_count = params.size();
        return new Timer(io_service_, params[0].numericValue(),
            param_count == 2 ? Php::Value() : params[2], callback,
            param_count < 4 ? true : params[3].boolValue());
    }

    Php::Value Service::add_tcp_server(Php::Parameters& params)
    {
        auto port = params[1].numericValue();
        if (port < 0 || port > 65535)
            throw Php::Exception("Bad port number.");
        auto param_count = params.size();
        auto server = new TcpServer(io_service_, param_count > 2,
            param_count < 4 ? Php::Value() : params[3],
            param_count == 2 ? Php::Value() : params[2]);
        server->init_acceptor(params[0].stringValue(), boost::numeric_cast<unsigned short>(port));
        return server;
    }

    Php::Value Service::add_unix_server(Php::Parameters& params)
    {
        auto param_count = params.size();
        auto server = new UnixServer(io_service_, param_count > 1,
            param_count < 3 ? Php::Value() : params[2],
            param_count == 1 ? Php::Value() : params[1]);
        server->init_acceptor(params[0].stringValue());
        return server;
    }

    Php::Value Service::add_signal(Php::Parameters& params)
    {
        auto param_count = params.size();
        auto signal = new Signal(io_service_, param_count == 1 ? Php::Value() : params[1], params[0]);
        if (param_count > 2)
            signal->add({ params.begin() + 2, params.end() });
        return signal;
    }

    Php::Value Service::run()
    {
        return boost::numeric_cast<int64_t>(io_service_.run());
    }

    Php::Value Service::run_one()
    {
        return boost::numeric_cast<int64_t>(io_service_.run_one());
    }

    Php::Value Service::poll()
    {
        return boost::numeric_cast<int64_t>(io_service_.poll());
    }

    Php::Value Service::poll_one()
    {
        return boost::numeric_cast<int64_t>(io_service_.poll_one());
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
            callback(argument);
        });
    }

}