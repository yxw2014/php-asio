/**
 * php-asio/future.cpp
 *
 * @author CismonX<admin@cismon.net>
 */

#include "future.hpp"
#include "service.hpp"

namespace Asio
{
    void Future::resolve(const boost::system::error_code& ec, unsigned arg)
    {
        send_ = callback_(ec, arg);
        if (yield_)
        {
            Service::last_error_ = static_cast<int64_t>(ec.value());
            generator_.call("send", send_);
            coroutine(generator_);
        }
        delete wrapper_;
    }

    Future::Future(const std::function<Php::Value(const boost::system::error_code&, unsigned)>&& callback) : callback_(move(callback))
    {
        wrapper_ = new Php::Object("Asio\\Future", this);
    }

    void Future::coroutine(const Php::Value& generator)
    {
        if (generator.instanceOf("Generator") && generator.call("valid").boolValue())
        {
            auto value = generator.call("current");
            if (!value.instanceOf("Asio\\Future"))
                generator.call("throw", Php::Object("Exception", "Invalid yield value. Future expected."));
            auto future = static_cast<Future*>(value.implementation());
            future->generator_ = generator;
            future->yield_ = true;
        }
    }
}
