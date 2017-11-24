/**
 * php-asio/resolver.cpp
 *
 * @author CismonX<admin@cismon.net>
 */

#include "resolver.hpp"

namespace Asio
{
    template <typename Protocol>
    Php::Value Resolver<Protocol>::handler(const boost::system::error_code& error, iterator iter)
    {
        iterator end;
        std::vector<std::string> resolved;
        while (iter != end)
            resolved.push_back((*iter++).endpoint().address().to_string());
        Php::Value addr_list(resolved);
        if (callback_.isCallable())
            Future::coroutine(callback_(this, addr_list, error.value(), argument_));
        return addr_list;
    }

    template <typename Protocol>
    Future* Resolver<Protocol>::resolve(const typename Protocol::resolver::query&& query)
    {
        auto future = Future::add();
        future->on_resolve<iterator>(boost::bind(&Resolver::handler, this, _1, _2));
        resolver_.async_resolve(query, ASYNC_HANDLER_DOUBLE_ARG(iterator));
        return future;
    }

    template <>
    TcpResolver::Resolver(boost::asio::io_service& io_service) : Base(io_service), resolver_(io_service)
    {
        wrapper_ = new Php::Object("Asio\\TcpResolver", this);
    }

    template <>
    UdpResolver::Resolver(boost::asio::io_service& io_service) : Base(io_service), resolver_(io_service)
    {
        wrapper_ = new Php::Object("Asio\\UdpResolver", this);
    }

    template <typename Protocol>
    Php::Value Resolver<Protocol>::resolve(Php::Parameters& params)
    {
        if (cancelled_)
            throw Php::Exception("Trying to resolve on a cancelled resolver.");
        auto param_count = params.size();
        callback_ = param_count > 2 ? params[2] : Php::Value();
        argument_ = param_count > 3 ? params[3] : Php::Value();
        return resolve({ params[0].stringValue(), param_count > 1 ? params[1].stringValue() : "" });
    }

    template <typename Protocol>
    void Resolver<Protocol>::cancel()
    {
        if (cancelled_)
            throw Php::Exception("Trying to cancel a cancelled resolver.");
        resolver_.cancel();
        cancelled_ = true;
    }

    // Instantiation for resolver.
    template class Resolver<tcp>;
    template class Resolver<udp>;
}
