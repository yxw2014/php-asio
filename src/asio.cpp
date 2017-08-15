#include "includes.hpp"

Php::Value Asio::addTimer(Php::Parameters& params)
{
    auto callback = params[1];
    if (!callback.isCallable())
        throw Php::Exception("Timer callback not callable.");
    bool persistent;
    auto param_count = params.size();
    if (param_count < 4)
    {
        if (param_count == 2)
            params.push_back(Php::Value());
        persistent = true;
    }
    else
        persistent = params[3].boolValue();
    auto timer = new Timer(_io_service, params[0].numericValue(), params[2], callback, persistent);
    return timer->getId();
}

void Asio::delTimer(Php::Parameters& params) const
{
    auto timer_id = params[0].numericValue();
    Timer::cancel(_io_service.getId(), timer_id);
}

Php::Value Asio::run()
{
    return boost::lexical_cast<int64_t>(_io_service().run());
}

Php::Value Asio::runOne()
{
    return boost::lexical_cast<int64_t>(_io_service().run_one());
}

Php::Value Asio::poll()
{
    return boost::lexical_cast<int64_t>(_io_service().poll());
}

Php::Value Asio::pollOne()
{
    return boost::lexical_cast<int64_t>(_io_service().poll_one());
}

void Asio::stop()
{
    _io_service().stop();
}

void Asio::reset()
{
    _io_service().reset();
}

Php::Value Asio::stopped()
{
    return _io_service().stopped();
}

void Asio::post(Php::Parameters& params)
{
    if (params.size() < 2)
        params.push_back(Php::Value());
    auto callback = params[0];
    if (!callback.isCallable())
        throw Php::Exception("Handler not callable.");
    auto argument = params[1];
    _io_service().post([callback, argument] ()
    {
        callback(argument);
    });
}