/**
 * php-asio/server.cpp
 *
 * @author CismonX<admin@cismon.net>
 */

#include "server.hpp"

namespace Asio
{
    template <typename Protocol>
    void Server<Protocol>::accept(void*)
    {
        auto socket = new Socket<Protocol>(io_service_);
        socket->wrap();
        acceptor_->async_accept(socket->getSocket(),
            boost::bind(&Server::_handler, this, boost::asio::placeholders::error, socket));
    }

    template <typename Protocol>
    void Server<Protocol>::_handler(const boost::system::error_code& error, Socket<Protocol>* const socket)
    {
        context_flag_ = true;
        callback_(this, socket, boost::numeric_cast<int64_t>(error.value()), argument_);
        context_flag_ = false;
        if (stopped_)
        {
            socket->unwrap();
            delete wrapper_;
        }
        else if (auto_accept_)
            accept();
    }

    template <typename Protocol>
    Server<Protocol>::Server(
        boost::asio::io_service& io_service,
        bool auto_accept,
        const Php::Value& argument,
        const Php::Value& callback
    ) : Base(io_service), argument_(argument), callback_(callback), auto_accept_(auto_accept) {}

    template <typename Protocol>
    Server<Protocol>::~Server()
    {
        stop();
    }

    template <>
    void TcpServer::init_acceptor(const std::string& address, unsigned short port)
    {
        using boost::asio::ip::tcp;
        try
        {
            tcp::endpoint endpoint(boost::asio::ip::address::from_string(address), port);
            acceptor_ = new tcp::acceptor(io_service_, endpoint);
        }
        catch (boost::system::system_error& error)
        {
            acceptor_ = nullptr;
            throw Php::Exception(std::string("Failed to start TCP server, error code: ") + std::to_string(error.code().value()));
        }
        wrapper_ = new Php::Object("Asio\\TcpServer", this);
        if (auto_accept_)
            accept();
    }

    template <>
    void UnixServer::init_acceptor(const std::string& path)
    {
        using unix = boost::asio::local::stream_protocol;
        try
        {
            // Unlink socket file before binding.
            if (boost::filesystem::status(path).type() == boost::filesystem::socket_file)
                boost::filesystem::remove(path);
            unix::endpoint endpoint(path);
            acceptor_ = new unix::acceptor(io_service_, endpoint);
        }
        catch (boost::system::system_error& error)
        {
            acceptor_ = nullptr;
            throw Php::Exception(std::string("Failed to start UNIX server, error code: ") + std::to_string(error.code().value()));
        }
        wrapper_ = new Php::Object("Asio\\UnixServer", this);
        if (auto_accept_)
            accept();
    }
    
    template <typename Protocol>
    void Server<Protocol>::accept(Php::Parameters& params)
    {
        if (stopped_)
            return;
        if (auto_accept_)
            throw Php::Exception("Server is working in auto-accept mode.");
        auto param_count = params.size();
        if (param_count)
        {
            callback_ = params[0];
            argument_ = param_count == 1 ? Php::Value() : params[1];
        }
        accept();
    }

    template <typename Protocol>
    void Server<Protocol>::stop()
    {
        if (!stopped_)
        {
            stopped_ = true;
            if (acceptor_)
            {
                acceptor_->cancel();
                acceptor_->close();
                delete acceptor_;
            }
        }
    }

    // Instantiation for TcpServer.
    template class Server<boost::asio::ip::tcp>;

    // Instantiation for UnixServer.
    template class Server<boost::asio::local::stream_protocol>;
}