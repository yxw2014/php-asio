/**
 * php-asio/server.cpp
 *
 * @author CismonX<admin@cismon.net>
 */

#include "server.hpp"

namespace Asio
{
    template <typename protocol>
    void Server<protocol>::_accept()
    {
        auto socket = new Socket<protocol>(_io_service);
        socket->wrap();
        _acceptor->async_accept(socket->getSocket(),
            boost::bind(&Server::_handler, this, boost::asio::placeholders::error, socket));
    }

    template <typename protocol>
    void Server<protocol>::_handler(const boost::system::error_code& error, Socket<protocol>* const socket)
    {
        _context_flag = true;
        _callback(this, socket, boost::numeric_cast<int64_t>(error.value()), _argument);
        _context_flag = false;
        if (_stopped)
        {
            socket->unwrap();
            delete _wrapper;
        }
        else if (_auto_accept)
            _accept();
    }

    template <typename protocol>
    Server<protocol>::Server(
        boost::asio::io_service& io_service,
        bool auto_accept,
        const Php::Value& argument,
        const Php::Value& callback
    ) : Base(io_service), _argument(argument), _callback(callback), _auto_accept(auto_accept) {}

    template <typename protocol>
    Server<protocol>::~Server()
    {
        if (_acceptor != nullptr)
        {
            _acceptor->close();
            delete _acceptor;
        }
    }

    template <>
    void TcpServer::initAcceptor(const std::string& address, unsigned short port)
    {
        using boost::asio::ip::tcp;
        try
        {
           tcp::endpoint endpoint(boost::asio::ip::address::from_string(address), port);
            _acceptor = new tcp::acceptor(_io_service, endpoint);
        }
        catch (boost::system::system_error& error)
        {
            _acceptor = nullptr;
            throw Php::Exception(std::string("Failed to start TCP server, error code: ") + std::to_string(error.code().value()));
        }
        _wrapper = new Php::Object("Asio\\TcpServer", this);
        if (_auto_accept)
            _accept();
    }

    template <>
    void UnixServer::initAcceptor(const std::string& path)
    {
        using boost::asio::local::stream_protocol;
        try
        {
            //Unlink socket file before binding.
            boost::filesystem::remove(path);
            stream_protocol::endpoint endpoint(path);
            _acceptor = new stream_protocol::acceptor(_io_service, endpoint);
        }
        catch (boost::system::system_error& error)
        {
            _acceptor = nullptr;
            throw Php::Exception(std::string("Failed to start UNIX server, error code: ") + std::to_string(error.code().value()));
        }
        _wrapper = new Php::Object("Asio\\UnixServer", this);
        if (_auto_accept)
            _accept();
    }
    
    template <typename protocol>
    void Server<protocol>::accept(Php::Parameters& params)
    {
        if (_auto_accept)
            throw Php::Exception("Server is working in auto-accept mode.");
        auto param_count = params.size();
        if (param_count)
        {
            _callback = params[0];
            _argument = param_count == 1 ? Php::Value() : params[1];
        }
        _accept();
    }

    template <typename protocol>
    void Server<protocol>::stop()
    {
        _acceptor->cancel();
        _stopped = true;
    }

    // Instantiation for TcpServer.
    template class Server<boost::asio::ip::tcp>;

    // Instantiation for UnixServer.
    template class Server<boost::asio::local::stream_protocol>;
}