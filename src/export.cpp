#include "includes.hpp"

using namespace Asio;

extern "C" PHPCPP_EXPORT void* get_module()
{
    static Php::Extension asio("asio", "1.0");

    //Class Asio\Service.
    Php::Class<Service> service("Asio\\Service");

    //Methods for creating IO objects.
    service.method<&Service::addTimer>("addTimer", {
        Php::ByVal("interval", Php::Type::Numeric),
        Php::ByVal("callback", Php::Type::Callable),
        Php::ByVal("argument", Php::Type::Null, false),
        Php::ByVal("persistent", Php::Type::Bool, false)
    });
    service.method<&Service::addTcpServer>("addTcpServer", {
        Php::ByVal("address", Php::Type::String),
        Php::ByVal("port", Php::Type::Numeric),
        Php::ByVal("callback", Php::Type::Callable, false),
        Php::ByVal("argument", Php::Type::Null, false),
    });

    //Methods for execution control.
    service.method<&Service::run>("run");
    service.method<&Service::runOne>("runOne");
    service.method<&Service::poll>("poll");
    service.method<&Service::pollOne>("pollOne");
    service.method<&Service::stop>("stop");
    service.method<&Service::stopped>("stopped");
    service.method<&Service::reset>("reset");
    service.method<&Service::post>("post", {
        Php::ByVal("callback", Php::Type::Callable),
        Php::ByVal("argument", Php::Type::Null, false)
    });

    //Class Asio\Timer.
    Php::Class<Timer> timer("Asio\\Timer", Php::Final);
    //timer.method<&Timer::__construct>("__construct", Php::Private);

    //Methods for timer.
    timer.method<&Timer::defer>("defer", {
        Php::ByVal("interval", Php::Type::Numeric, false),
        Php::ByVal("callback", Php::Type::Callable, false),
        Php::ByVal("argument", Php::Type::Null, false)
    });
    timer.method<&Timer::cancel>("cancel");

    //Class Asio\TcpServer.
    Php::Class<TcpServer> tcp_server("Asio\\TcpServer", Php::Final);

    //Methods for TCP server.
    tcp_server.method<&TcpServer::accept>("accept", {
        Php::ByVal("callback", Php::Type::Callable, false),
        Php::ByVal("argument", Php::Type::Null, false)
    });
    tcp_server.method<&TcpServer::stop>("stop");

    //Class Asio\TcpSocket.
    Php::Class<TcpSocket> tcp_connection("Asio\\TcpSocket", Php::Final);

    //Methods for reading and writing.
    tcp_connection.method<&TcpSocket::read>("read", {
        Php::ByVal("length", Php::Type::Numeric),
        Php::ByVal("read_some", Php::Type::Bool),
        Php::ByVal("callback", Php::Type::Callable),
        Php::ByVal("argument", Php::Type::Null, false),
    });
    tcp_connection.method<&TcpSocket::write>("write", {
        Php::ByVal("data", Php::Type::String),
        Php::ByVal("write_some", Php::Type::Bool),
        Php::ByVal("callback", Php::Type::Callable, false),
        Php::ByVal("argument", Php::Type::Null, false),
    });

    //Other Methods.
    tcp_connection.method<&TcpSocket::available>("available");
    tcp_connection.method<&TcpSocket::atMark>("atMark");
    tcp_connection.method<&TcpSocket::close>("close");

    asio.add(std::move(service));
    asio.add(std::move(timer));
    asio.add(std::move(tcp_server));
    asio.add(std::move(tcp_connection));
    return asio;
}