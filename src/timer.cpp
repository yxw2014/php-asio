/**
 * php-asio/timer.cpp
 *
 * @author CismonX<admin@cismon.net>
 */

#include "timer.hpp"

namespace Asio
{
    void Timer::defer(void*)
    {
        timer_.expires_from_now(boost::posix_time::milliseconds(interval_));
        timer_.async_wait(boost::bind(&Timer::handler, this, boost::asio::placeholders::error));
    }

    void Timer::handler(const boost::system::error_code& error)
    {
        context_flag_ = true;
        callback_(this, argument_, boost::numeric_cast<int64_t>(error.value()));
        //Errno 125: Operation cancelled.
        if (error.value() == 125)
            persistent_ = false;
        if (persistent_ || !context_flag_)
        {
            context_flag_ = false;
            defer();
        }
        // Delete this Php::Object when timer stops.
        // Refcount will fall to zero and timer will be destroyed.
        else
        {
            context_flag_ = false;
            delete wrapper_;
            wrapper_ = nullptr;
        }
    }

    Timer::Timer(
        boost::asio::io_service& io_service,
        int64_t interval,
        const Php::Value& argument,
        const Php::Value& callback,
        bool persistent
    ) : Base(io_service), timer_(io_service), interval_(interval), argument_(argument), callback_(callback), persistent_(persistent)
    {
        // Objects instantiated in C++ must be wrapped within a Php::Object to make it accessible by Zend Engine.
        // Store this Php::Object in class property to make sure it stays alive until timer stopped.
        // Otherwise the refcount of wrapped object will fall to zero and object will be destroyed by Zend Engine.
        // The C++ object will also be destroyed by PHP-CPP, which will cause segmentation fault.
        wrapper_ = new Php::Object("Asio\\Timer", this);
        defer();
    }

    void Timer::defer(Php::Parameters& params)
    {
        if (!context_flag_)
            throw Php::Exception("Manual defer is not allowed outside handler callback.");
        context_flag_ = false;
        auto param_count = params.size();
        if (param_count)
        {
            interval_ = params[0];
            if (param_count > 1)
            {
                callback_ = params[1];
                argument_ = param_count == 2 ? Php::Value() : params[2];
            }
        }
    }

    void Timer::cancel()
    {
        if (context_flag_)
            persistent_ = false;
        else
            timer_.cancel();
    }

}