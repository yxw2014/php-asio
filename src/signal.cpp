/**
 * php-asio/signal.cpp
 *
 * @author CismonX<admin@cismon.net>
 */

#include "signal.hpp"

namespace Asio
{
    CORO_RETVAL Signal::handler(
        const boost::system::error_code& error,
        int signal,
        const Php::Value& callback,
        const Php::Value& argument)
    {
        if (callback.isCallable())
            CORO_REGISTER(callback(this, signal, error.value(), argument));
        CORO_RETURN(signal);
    }

    Future* Signal::wait(const Php::Value& callback, const Php::Value& argument)
    {
        auto future = Future::add();
        future->on_resolve<int>(boost::bind(&Signal::handler, this, _1, _2, callback, argument));
        signal_.async_wait(ASYNC_HANDLER_DOUBLE_ARG(int));
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
        boost::system::error_code ec;
        for (const auto& param : params) {
            if (!param.isNumeric())
                throw Php::Exception("Integer value expected.");
            auto signal = param.numericValue();
            signal_.add(static_cast<int>(signal), ec);
            if (ec)
                break;
        }
        return ec.value();
    }

    Php::Value Signal::remove(Php::Parameters& params)
    {
        if (cancelled_)
            throw Php::Exception("Trying to remove signal on a cancelled signal set.");
        boost::system::error_code ec;
        for (const auto& param : params) {
            if (!param.isNumeric())
                throw Php::Exception("Integer value expected.");
            auto signal = param.numericValue();
            signal_.remove(static_cast<int>(signal));
            if (ec)
                break;
        }
        return ec.value();
    }

    FUTURE_RETVAL Signal::wait(Php::Parameters& params)
    {
        if (cancelled_)
            throw Php::Exception("Trying to wait for a cancelled signal set.");
        auto param_count = params.size();
        FUTURE_RETURN wait(param_count ? params[0] : Php::Value(), param_count > 1 ? params[1] : Php::Value());
    }

    Php::Value Signal::clear()
    {
        if (cancelled_)
            throw Php::Exception("Trying to clear a cancelled signal set.");
        boost::system::error_code ec;
        signal_.clear(ec);
        return ec.value();
    }

    Php::Value Signal::cancel()
    {
        if (cancelled_)
            throw Php::Exception("Trying to cancel a cancelled signal set.");
        boost::system::error_code ec;
        signal_.cancel(ec);
        if (!ec) {
            cancelled_ = true;
            delete wrapper_;
        }
        return ec.value();
    }
}
