/**
 * php-asio/signal.cpp
 *
 * @author CismonX<admin@cismon.net>
 */

#include "signal.hpp"

namespace Asio
{
    Php::Value Signal::handler(const boost::system::error_code& error, int signal)
    {
        auto sig_num = boost::numeric_cast<int64_t>(signal);
        if (callback_.isCallable())
            Future::coroutine(callback_(this, sig_num, argument_, boost::numeric_cast<int64_t>(error.value())));
        return sig_num;
    }

    Future* Signal::wait()
    {
        auto future = new Future(boost::bind(&Signal::handler, this, _1, _2));
        signal_.async_wait(PHP_ASIO_ASYNC_HANDLER(boost::asio::placeholders::signal_number));
        return future;
    }

    Signal::Signal(boost::asio::io_service& io_service) : Base(io_service), signal_(io_service)
    {
        wrapper_ = new Php::Object("Asio\\Signal", this);
    }

    Php::Value Signal::add(Php::Parameters& params)
    {
        if (cancelled_)
            throw Php::Exception("Trying to add signal to a cancelled signal set.");
        return add({ params.begin(), params.end() });
    }

    int64_t Signal::add(const std::vector<Php::Value>&& signals)
    {
        boost::system::error_code ec;
        for (const auto& param : signals)
        {
            if (!param.isNumeric())
                throw Php::Exception("Integer value expected.");
            auto signal = param.numericValue();
            signal_.add(boost::numeric_cast<int>(signal), ec);
            if (ec) break;
        }
        return boost::numeric_cast<int64_t>(ec.value());
    }

    Php::Value Signal::remove(Php::Parameters& params)
    {
        if (cancelled_)
            throw Php::Exception("Trying to remove signal on a cancelled signal set.");
        boost::system::error_code ec;
        for (const auto& param : params)
        {
            if (!param.isNumeric())
                throw Php::Exception("Integer value expected.");
            auto signal = param.numericValue();
            signal_.remove(boost::numeric_cast<int>(signal));
            if (ec) break;
        }
        return boost::numeric_cast<int64_t>(ec.value());
    }

    Php::Value Signal::wait(Php::Parameters& params)
    {
        if (cancelled_)
            throw Php::Exception("Trying to wait for a cancelled signal set.");
        auto param_count = params.size();
        callback_ = param_count ? params[0] : Php::Value();
        argument_ = param_count > 1 ? params[1] : Php::Value();
        return wait();
    }

    Php::Value Signal::clear()
    {
        if (cancelled_)
            throw Php::Exception("Trying to clear a cancelled signal set.");
        boost::system::error_code ec;
        signal_.clear(ec);
        return boost::numeric_cast<int64_t>(ec.value());
    }

    Php::Value Signal::cancel()
    {
        if (cancelled_)
            throw Php::Exception("Trying to cancel a cancelled signal set.");
        boost::system::error_code ec;
        signal_.cancel(ec);
        delete wrapper_;
        return boost::numeric_cast<int64_t>(ec.value());
    }
}
