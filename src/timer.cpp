/**
 * php-asio/timer.cpp
 *
 * @author CismonX<admin@cismon.net>
 */

#include "timer.hpp"

namespace Asio
{
    Future* Timer::wait()
    {
        auto future = new Future(boost::bind(&Timer::handler, this, _1));
        timer_.async_wait(PHP_ASIO_ASYNC_HANDLER_SINGLE_ARG);
        return future;
    }

    Php::Value Timer::handler(const boost::system::error_code& error)
    {
        if (callback_.isCallable())
            Future::coroutine(callback_(this, argument_, static_cast<int64_t>(error.value())));
        return Php::Value();
    }

    Timer::Timer(boost::asio::io_service& io_service) : Base(io_service), timer_(io_service)
    {
        // Objects instantiated in C++ must be wrapped within a Php::Object to make it accessible by Zend Engine.
        // Store this Php::Object in class property to make sure it stays alive until timer stopped.
        // Otherwise the refcount of wrapped object will fall to zero and object will be destroyed by Zend Engine.
        // The C++ object will also be destroyed by PHP-CPP, which will cause segmentation fault.
        wrapper_ = new Php::Object("Asio\\Timer", this);
    }

    Php::Value Timer::expire(Php::Parameters& params)
    {
        auto time = params[0].numericValue();
        boost::system::error_code ec;
        if (params.size() == 2 && params[1].boolValue())
            timer_.expires_at(boost::posix_time::from_time_t(time / 1000) + boost::posix_time::millisec(time % 1000), ec);
        else
            timer_.expires_from_now(boost::posix_time::millisec(time), ec);
        return static_cast<int64_t>(ec.value());
    }

    Php::Value Timer::wait(Php::Parameters& params)
    {
        if (cancelled_)
            throw Php::Exception("Trying to wait for a cancelled timer.");
        auto param_count = params.size();
        callback_ = param_count ? params[0] : Php::Value();
        argument_ = param_count > 1 ? params[1] : Php::Value();
        return wait();
    }

    Php::Value Timer::cancel()
    {
        if (cancelled_)
            throw Php::Exception("Trying to cancel a cancelled timer.");
        boost::system::error_code ec;
        timer_.cancel(ec);
        if (!ec) {
            delete wrapper_;
            cancelled_ = true;
        }
        return static_cast<int64_t>(ec.value());
    }
}
