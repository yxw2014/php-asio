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
        timer_.expires_from_now(boost::posix_time::milliseconds(interval_));
        auto future = new Future(boost::bind(&Timer::handler, this, boost::asio::placeholders::error));
        timer_.async_wait(PHP_ASIO_ASYNC_HANDLER_NOARG);
        return future;
    }

    Php::Value Timer::handler(const boost::system::error_code& error)
    {
        if (callback_.isCallable())
            Future::coroutine(callback_(this, argument_, boost::numeric_cast<int64_t>(error.value())));
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

    Php::Value Timer::wait(Php::Parameters& params)
    {
        if (cancelled_)
            throw Php::Exception("Trying to wait for a cancelled timer.");
        interval_ = params[0];
        auto param_count = params.size();
        callback_ = param_count > 1 ? params[1] : Php::Value();
        argument_ = param_count > 2 ? params[2] : Php::Value();
        return wait();
    }

    void Timer::cancel()
    {
        if (!cancelled_)
        {
            cancelled_ = true;
            timer_.cancel();
            delete wrapper_;
        }
    }

}
