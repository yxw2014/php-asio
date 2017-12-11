/**
 * php-asio/future.cpp
 *
 * @author CismonX<admin@cismon.net>
 */

#include "future.hpp"

namespace Asio
{
    Future::Future()
    {
        wrapper_ = new Php::Object("Asio\\Future", this);
    }

    template <typename T>
    void Future::on_resolve(const ASYNC_CALLBACK(T)&& callback)
    {
        callback_ = new ASYNC_CALLBACK(T)(std::move(callback));
    }

    template <typename T>
    void Future::resolve(const boost::system::error_code& ec, T arg)
    {
        auto callback = static_cast<ASYNC_CALLBACK(T)*>(callback_);
#ifdef ENABLE_COROUTINE
        send_ = (*callback)(ec, arg);
        if (yield_) {
            last_error_ = static_cast<int64_t>(ec.value());
            generator_.call("send", send_);
            coroutine(generator_);
        }
#else // ENABLE_COROUTINE
        (*callback)(ec, arg);
#endif
        delete callback;
        delete wrapper_;
    }

#ifdef ENABLE_STRAND
    Php::Value Future::strand(const Php::Value& callback)
    {
        if (callback.instanceOf("Asio\\WrappedHandler")) {
            auto wrapped_hander = static_cast<WrappedHandler*>(callback.implementation());
            strand_ = &wrapped_hander->strand_;
            return wrapped_hander->callback_;
        }
        return callback;
    }
#endif // ENABLE_STRAND

#ifdef ENABLE_COROUTINE
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
#endif // ENABLE_COROUTINE

    template void Future::on_resolve(const ASYNC_CALLBACK(int)&&);
    template void Future::on_resolve(const ASYNC_CALLBACK(size_t)&&);
    template void Future::on_resolve(const ASYNC_CALLBACK(tcp::resolver::iterator)&&);
    template void Future::on_resolve(const ASYNC_CALLBACK(udp::resolver::iterator)&&);
    template void Future::resolve(const boost::system::error_code&, int);
    template void Future::resolve(const boost::system::error_code&, size_t);
    template void Future::resolve(const boost::system::error_code&, tcp::resolver::iterator);
    template void Future::resolve(const boost::system::error_code&, udp::resolver::iterator);

}
