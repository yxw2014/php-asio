/**
 * php-asio/signal.cpp
 *
 * @author CismonX<admin@cismon.net>
 */

#include "signal.hpp"

namespace Asio
{
    void Signal::_handler(const boost::system::error_code& error, int signal)
    {
        _callback(this, boost::numeric_cast<int64_t>(signal), _argument, boost::numeric_cast<int64_t>(error.value()));
        if (!_cancelled)
            _wait();
    }

    void Signal::_wait()
    {
        _signal.async_wait(boost::bind(&Signal::_handler, this,
            boost::asio::placeholders::error, boost::asio::placeholders::signal_number));
    }

    Signal::Signal(
        boost::asio::io_service& io_service,
        const Php::Value& argument,
        const Php::Value& callback
    ) : Base(io_service), _signal(io_service), _argument(argument), _callback(callback)
    {
        _wrapper = new Php::Object("Asio\\Signal", this);
        _wait();
    }

    void Signal::add(Php::Parameters& params)
    {
        add({ params.begin(), params.end() });
    }

    void Signal::add(const std::vector<Php::Value>&& signals)
    {
        for (const auto& param : signals)
        {
            if (!param.isNumeric())
                throw Php::Exception("Integer value expected.");
            auto signal = param.numericValue();
            try
            {
                _signal.add(boost::numeric_cast<int>(signal));
            }
            catch (boost::system::system_error& error)
            {
                throw Php::Exception(std::string("Failed to add signal ")
                    + std::to_string(signal) + ", error code: " + std::to_string(error.code().value()));
            }
        }
    }

    void Signal::remove(Php::Parameters& params)
    {
        for (auto param : params)
        {
            if (!param.isNumeric())
                throw Php::Exception("Integer value expected.");
            auto signal = param.numericValue();
            try
            {
                _signal.remove(boost::numeric_cast<int>(signal));
            }
            catch (boost::system::system_error& error)
            {
                throw Php::Exception(std::string("Failed to remove signal ")
                    + std::to_string(signal) + ", error code: " + std::to_string(error.code().value()));
            }
        }
    }

    void Signal::clear()
    {
        try
        {
            _signal.clear();
        }
        catch (boost::system::system_error& error)
        {
            throw Php::Exception(std::string("Failed to clear signals, error code: " + std::to_string(error.code().value())));
        }
    }

    void Signal::cancel()
    {
        if (_cancelled)
            return;
        try
        {
            _signal.cancel();
        }
        catch (boost::system::system_error& error)
        {
            throw Php::Exception(std::string("Failed to cancel signal handler, error code: " + std::to_string(error.code().value())));
        }
        _cancelled = true;
        delete _wrapper;
    }
}
