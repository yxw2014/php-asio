/**
 * php-asio/acceptor.cpp
 *
 * @author CismonX<admin@cismon.net>
 */

#include "acceptor.hpp"

namespace Asio
{
    template <typename Protocol>
    Future* Acceptor<Protocol>::accept(const Php::Value& callback, const Php::Value& argument)
    {
        auto socket = new Socket<Protocol>(io_service_);
        auto future = Future::add();
        future->on_resolve<NOARG>(boost::bind(&Acceptor::handler, this, _1, socket, STRAND_UNWRAP, argument));
        acceptor_.async_accept(socket->get_socket(), STRAND_RESOLVE(ASYNC_HANDLER_SINGLE_ARG));
        return future;
    }

    template <typename Protocol>
    CORO_RETVAL Acceptor<Protocol>::handler(
        const boost::system::error_code& error,
        Socket<Protocol>* const socket,
        const Php::Value& callback,
        const Php::Value& argument)
    {
        if (callback.isCallable())
            CORO_REGISTER(callback(this, socket, error.value(), argument));
        CORO_RETURN(socket);
    }

    template <>
    TcpAcceptor::Acceptor(boost::asio::io_service& io_service) : Base(io_service), acceptor_(io_service)
    {
        wrapper_ = new Php::Object("Asio\\TcpAcceptor", this);
    }

    template <>
    UnixAcceptor::Acceptor(boost::asio::io_service& io_service) : Base(io_service), acceptor_(io_service)
    {
        wrapper_ = new Php::Object("Asio\\UnixAcceptor", this);
    }

    template <typename Protocol> template <typename P, std::string(P::endpoint::*port)()>
    Php::Value Acceptor<Protocol>::open_inet(Php::Parameters& params)
    {
        boost::system::error_code ec;
        acceptor_.open(params[0].boolValue() ? Protocol::v6() : Protocol::v4(), ec);
        return ec.value();
    }

    template <typename Protocol> template <typename P, std::string(P::endpoint::*path)()>
    Php::Value Acceptor<Protocol>::open_local()
    {
        boost::system::error_code ec;
        acceptor_.open(Protocol(), ec);
        return ec.value();
    }

    template <>
    Php::Value TcpAcceptor::bind(Php::Parameters& params)
    {
        boost::system::error_code ec;
        acceptor_.bind({ boost::asio::ip::address::from_string(params[0].stringValue()),
            static_cast<unsigned short>(params[1].numericValue()) }, ec);
        return ec.value();
    }

    template <>
    Php::Value UnixAcceptor::bind(Php::Parameters& params)
    {
        auto path = params[0].stringValue();
        if (boost::filesystem::status(path).type() == boost::filesystem::socket_file)
            boost::filesystem::remove(path);
        boost::system::error_code ec;
        acceptor_.bind({ path }, ec);
        return ec.value();
    }

    template <typename Protocol>
    Php::Value Acceptor<Protocol>::listen(Php::Parameters& params)
    {
        boost::system::error_code ec;
        acceptor_.listen(params.size() ? static_cast<int>(params[1].numericValue()) : Protocol::socket::max_connections, ec);
        return ec.value();
    }

    template <typename Protocol>
    FUTURE_RETVAL Acceptor<Protocol>::accept(Php::Parameters& params)
    {
        if (cancelled_)
            throw Php::Exception("Trying to accept connection on a closed acceptor.");
        auto param_count = params.size();
        FUTURE_RETURN accept(param_count ? params[0] : Php::Value(), param_count > 1 ? params[1] : Php::Value());
    }

    template <typename Protocol>
    Php::Value Acceptor<Protocol>::close()
    {
        if (!cancelled_)
            throw Php::Exception("Trying to close a closed acceptor.");
        boost::system::error_code ec;
        acceptor_.cancel(ec);
        if (!ec) {
            acceptor_.close(ec);
            if (!ec) {
                delete wrapper_;
                cancelled_ = true;
            }
        }
        return ec.value();
    }

    template class Acceptor<tcp>;
    template Php::Value TcpAcceptor::open_inet(Php::Parameters&);

    template class Acceptor<unix>;
    template Php::Value UnixAcceptor::open_local();
}
