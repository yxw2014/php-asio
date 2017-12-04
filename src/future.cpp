/**
 * php-asio/future.cpp
 *
 * @author CismonX<admin@cismon.net>
 */

#include "future.hpp"
#include "service.hpp"

namespace Asio
{
    template <typename T>
    void Future::on_resolve(const ASYNC_CALLBACK(T)&& callback)
    {
        callback_ = new ASYNC_CALLBACK(T)(std::move(callback));
    }

    template <typename T>
    void Future::resolve(const boost::system::error_code& ec, T arg)
    {
        auto callback = static_cast<ASYNC_CALLBACK(T)*>(callback_);
#if ENABLE_COROUTINE
        send_ = (*callback)(ec, arg);
        if (yield_) {
            last_error_ = static_cast<int64_t>(ec.value());
            generator_.call("send", send_);
            coroutine(generator_);
        }
#else
        (*callback)(ec, arg);
#endif
        delete callback;
        delete wrapper_;
    }

    Future::Future()
    {
        wrapper_ = new Php::Object("Asio\\Future", this);
    }

#if ENABLE_COROUTINE
    void Future::coroutine(const Php::Value& generator)
    {
        if (generator.instanceOf("Generator") && generator.call("valid").boolValue()) {
            auto value = generator.call("current");
            if (!value.instanceOf("Asio\\Future"))
                generator.call("throw", Php::Object("Exception", "Invalid yield value. Future expected."));
            auto future = static_cast<Future*>(value.implementation());
            future->generator_ = generator;
            future->yield_ = true;
        }
    }

    thread_local int64_t Future::last_error_ = 0;
#endif

    // Initialization for Future.
    template void Future::on_resolve(const ASYNC_CALLBACK(int)&&);
    template void Future::resolve(const boost::system::error_code&, int);
    template void Future::on_resolve(const ASYNC_CALLBACK(size_t)&&);
    template void Future::resolve(const boost::system::error_code&, size_t);
    template void Future::on_resolve(const ASYNC_CALLBACK(tcp::resolver::iterator)&&);
    template void Future::resolve(const boost::system::error_code&, tcp::resolver::iterator);
    template void Future::on_resolve(const ASYNC_CALLBACK(udp::resolver::iterator)&&);
    template void Future::resolve(const boost::system::error_code&, udp::resolver::iterator);
}
