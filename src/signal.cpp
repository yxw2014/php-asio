/**
 * php-asio/signal.cpp
 *
 * @author CismonX<admin@cismon.net>
 */

#include "signal.hpp"

namespace Asio
{
    void Signal::handler(const boost::system::error_code& error, int signal)
    {
        callback_(this, boost::numeric_cast<int64_t>(signal), argument_, boost::numeric_cast<int64_t>(error.value()));
        if (!cancelled_)
            wait();
    }

    void Signal::wait()
    {
        signal_.async_wait(boost::bind(&Signal::handler, this,
            boost::asio::placeholders::error, boost::asio::placeholders::signal_number));
    }

    Signal::Signal(
        boost::asio::io_service& io_service,
        const Php::Value& argument,
        const Php::Value& callback
    ) : Base(io_service), signal_(io_service), argument_(argument), callback_(callback)
    {
        wrapper_ = new Php::Object("Asio\\Signal", this);
        wait();
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
                signal_.add(boost::numeric_cast<int>(signal));
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
                signal_.remove(boost::numeric_cast<int>(signal));
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
            signal_.clear();
        }
        catch (boost::system::system_error& error)
        {
            throw Php::Exception(std::string("Failed to clear signals, error code: " + std::to_string(error.code().value())));
        }
    }

    void Signal::cancel()
    {
        if (cancelled_)
            return;
        try
        {
            signal_.cancel();
        }
        catch (boost::system::system_error& error)
        {
            throw Php::Exception(std::string("Failed to cancel signal handler, error code: " + std::to_string(error.code().value())));
        }
        cancelled_ = true;
        delete wrapper_;
    }
}
