/**
 * php-asio/server.cpp
 *
 * @author CismonX<admin@cismon.net>
 */

#include "server.hpp"

namespace Asio
{
    template <typename Protocol>
    Future* Server<Protocol>::accept()
    {
        auto socket = new Socket<Protocol>(io_service_);
        socket->wrap();
        auto future = new Future(boost::bind(&Server::handler, this, _1, socket));
        acceptor_->async_accept(socket->getSocket(), PHP_ASIO_ASYNC_HANDLER_SINGLE_ARG);
        return future;
    }

    template <typename Protocol>
    Php::Value Server<Protocol>::handler(const boost::system::error_code& error, Socket<Protocol>* const socket)
    {
        if (callback_.isCallable())
            Future::coroutine(callback_(this, socket, static_cast<int64_t>(error.value()), argument_));
        return socket;
    }

    template <typename Protocol>
    Server<Protocol>::Server(boost::asio::io_service& io_service) : Base(io_service) {}

    template <>
    void TcpServer::init_acceptor(const std::string& address, unsigned short port)
    {
        using boost::asio::ip::tcp;
        try {
            tcp::endpoint endpoint(boost::asio::ip::address::from_string(address), port);
            acceptor_ = new tcp::acceptor(io_service_, endpoint);
        } catch (boost::system::system_error& error) {
            acceptor_ = nullptr;
            throw Php::Exception(std::string("Failed to start TCP server, error code: ") + std::to_string(error.code().value()));
        }
        wrapper_ = new Php::Object("Asio\\TcpServer", this);
    }

    template <>
    void UnixServer::init_acceptor(const std::string& path)
    {
        using unix = boost::asio::local::stream_protocol;
        try {
            // Unlink socket file before binding.
            if (boost::filesystem::status(path).type() == boost::filesystem::socket_file)
                boost::filesystem::remove(path);
            unix::endpoint endpoint(path);
            acceptor_ = new unix::acceptor(io_service_, endpoint);
        } catch (boost::system::system_error& error) {
            acceptor_ = nullptr;
            throw Php::Exception(std::string("Failed to start UNIX server, error code: ") + std::to_string(error.code().value()));
        }
        wrapper_ = new Php::Object("Asio\\UnixServer", this);
    }
    
    template <typename Protocol>
    Php::Value Server<Protocol>::accept(Php::Parameters& params)
    {
        if (cancelled_)
            throw Php::Exception("Trying to accept connection on a stopped server.");
        auto param_count = params.size();
        callback_ = param_count ? params[0] : Php::Value();
        argument_ = param_count > 1 ? params[1] : Php::Value();
        return accept();
    }

    template <typename Protocol>
    Php::Value Server<Protocol>::stop()
    {
        if (!cancelled_)
            throw Php::Exception("Trying to stop a stopped server.");
        boost::system::error_code ec;
        acceptor_->cancel(ec);
        if (!ec) {
            acceptor_->close(ec);
            if (!ec) {
                delete acceptor_;
                delete wrapper_;
                cancelled_ = true;
            }
        }
        return static_cast<int64_t>(ec.value());
    }

    // Instantiation for TcpServer.
    template class Server<boost::asio::ip::tcp>;

    // Instantiation for UnixServer.
    template class Server<boost::asio::local::stream_protocol>;
}
