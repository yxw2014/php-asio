/**
 * php-asio/strand.cpp
 *
 * @author CismonX<admin@cismon.net>
 */

#include "strand.hpp"

#ifdef ENABLE_STRAND
namespace Asio
{
    Strand::Strand(boost::asio::io_service& io_service) : strand_(io_service)
    {
        wrapper_ = new Php::Object("Asio\\Strand", this);
    }

    void Strand::dispatch(Php::Parameters& params)
    {
        auto callback = params[0];
        auto argument = params.size() == 1 ? Php::Value() : params[1];
        strand_.dispatch([callback, argument]()
        {
            CORO_REGISTER(callback(argument));
        });
    }

    void Strand::post(Php::Parameters& params)
    {
        auto callback = params[0];
        auto argument = params.size() == 1 ? Php::Value() : params[1];
        strand_.post([callback, argument]()
        {
            CORO_REGISTER(callback(argument));
        });
    }

    Php::Value Strand::running_in_this_thread() const
    {
        return strand_.running_in_this_thread();
    }

    Php::Value Strand::wrap(Php::Parameters& params)
    {
        return Php::Object("Asio\\WrappedHandler",
            new WrappedHandler(strand_, params.size() ? params[0] : Php::Value()));
    }

    void Strand::destroy()
    {
        if (wrapper_) {
            delete wrapper_;
            wrapper_ = nullptr;
        }
    }
}
#endif // ENABLE_STRAND
