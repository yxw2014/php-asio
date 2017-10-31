/**
 * php-asio/service.cpp
 *
 * @author CismonX<admin@cismon.net>
 */

#include "service.hpp"
#include "timer.hpp"
#include "tcp_server.hpp"
#include "signal.hpp"

namespace Asio
{
    Php::Value Service::addTimer(Php::Parameters& params)
    {
        auto callback = params[1];
        auto param_count = params.size();
        return new Timer(_io_service, params[0].numericValue(),
            param_count == 2 ? Php::Value() : params[2], callback,
            param_count < 4 ? true : params[3].boolValue());
    }

    Php::Value Service::addTcpServer(Php::Parameters& params)
    {
        auto port = params[1].numericValue();
        if (port < 0 || port > 65535)
            throw Php::Exception("Bad port number.");
        auto param_count = params.size();
        return new TcpServer(_io_service, params[0].stringValue(),
            boost::numeric_cast<unsigned short>(port), param_count > 2,
            param_count < 4 ? Php::Value() : params[3],
            param_count == 2 ? Php::Value() : params[2]);
    }

    Php::Value Service::addSignal(Php::Parameters& params)
    {
        auto param_count = params.size();
        auto signal = new Signal(_io_service, param_count == 1 ? Php::Value() : params[1], params[0]);
        if (param_count > 2)
            signal->add({ params.begin() + 2, params.end() });
        return signal;
    }

    Php::Value Service::run()
    {
        return boost::numeric_cast<int64_t>(_io_service.run());
    }

    Php::Value Service::runOne()
    {
        return boost::numeric_cast<int64_t>(_io_service.run_one());
    }

    Php::Value Service::poll()
    {
        return boost::numeric_cast<int64_t>(_io_service.poll());
    }

    Php::Value Service::pollOne()
    {
        return boost::numeric_cast<int64_t>(_io_service.poll_one());
    }

    void Service::stop()
    {
        _io_service.stop();
    }

    void Service::reset()
    {
        _io_service.reset();
    }

    Php::Value Service::stopped() const
    {
        return _io_service.stopped();
    }

    void Service::post(Php::Parameters& params)
    {
        auto callback = params[0];
        if (!callback.isCallable())
            throw Php::Exception("Handler not callable.");
        auto argument = params.size() == 1 ? Php::Value() : params[1];
        _io_service.post([callback, argument]()
        {
            callback(argument);
        });
    }

}