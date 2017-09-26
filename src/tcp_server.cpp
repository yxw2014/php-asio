#include "includes.hpp"

void TcpServer::_accept()
{
    auto connection = new TcpConnection(_io_service);
    _acceptor->async_accept(connection->getSocket(),
        boost::bind(&TcpServer::_handler, this, boost::asio::placeholders::error, connection));
}

void TcpServer::_handler(const boost::system::error_code& error, TcpConnection* connection)
{
    //Stop outside accept handler callback.
    if (_stopped)
    {
        delete _wrapper;
        return;
    }
    _context_flag = true;
    _callback(connection, error.value(), _argument);
    _context_flag = false;
    if (_stopped)
        delete _wrapper;
    else if (_auto_accept)
        _accept();
}

TcpServer::TcpServer(
    boost::asio::io_service& io_service,
    const std::string& address,
    unsigned short port,
    bool auto_accept,
    const Php::Value& argument,
    const Php::Value& callback
) : _io_service(io_service), _argument(argument), _callback(callback), _auto_accept(auto_accept)
{
    try
    {
        tcp::endpoint endpoint(boost::asio::ip::address::from_string(address), port);
        _acceptor = new tcp::acceptor(io_service, endpoint);
    }
    catch (boost::system::system_error& error)
    {
        _acceptor = nullptr;
        throw Php::Exception(std::string("Failed to start TCP server, error code: ") + std::to_string(error.code().value()));
    }
    _wrapper = new Php::Object("Asio\\TcpServer", this);
    if (auto_accept)
        _accept();
}

TcpServer::~TcpServer()
{
    if (_acceptor != nullptr)
    {
        _acceptor->close();
        delete _acceptor;
    }
}

void TcpServer::accept(Php::Parameters& params)
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

void TcpServer::stop()
{
    _stopped = true;
}