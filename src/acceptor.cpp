/**
 * php-asio/acceptor.cpp
 *
 * @author CismonX<admin@cismon.net>
 */

#include "acceptor.hpp"

namespace Asio
{
    template <typename Protocol>
    Future* Acceptor<Protocol>::accept()
    {
        auto socket = new Socket<Protocol>(io_service_);
        auto future = Future::add();
        future->on_resolve<NOARG>(boost::bind(&Acceptor::handler, this, _1, socket));
        acceptor_.async_accept(socket->getSocket(), ASYNC_HANDLER_SINGLE_ARG);
        return future;
    }

    template <typename Protocol>
    Php::Value Acceptor<Protocol>::handler(const boost::system::error_code& error, Socket<Protocol>* const socket)
    {
        if (callback_.isCallable())
            Future::coroutine(callback_(this, socket, error.value(), argument_));
        return socket;
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
    Php::Value Acceptor<Protocol>::accept(Php::Parameters& params)
    {
        if (cancelled_)
            throw Php::Exception("Trying to accept connection on a stopped acceptor.");
        auto param_count = params.size();
        callback_ = param_count ? params[0] : Php::Value();
        argument_ = param_count > 1 ? params[1] : Php::Value();
        return accept();
    }

    template <typename Protocol>
    Php::Value Acceptor<Protocol>::stop()
    {
        if (!cancelled_)
            throw Php::Exception("Trying to stop a stopped acceptor.");
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

    // Instantiation for TCP acceptor.
    template class Acceptor<tcp>;
    template Php::Value TcpAcceptor::open_inet(Php::Parameters&);

    // Instantiation for UNIX domain socket acceptor.
    template class Acceptor<unix>;
    template Php::Value UnixAcceptor::open_local();
}
