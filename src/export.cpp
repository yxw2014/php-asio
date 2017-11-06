/**
 * php-asio/export.cpp
 *
 * @author CismonX<admin@cismon.net>
 */

#include <phpcpp.h>

#include "service.hpp"
#include "timer.hpp"
#include "server.hpp"
#include "socket.hpp"
#include "signal.hpp"

using namespace Asio;

extern "C" PHPCPP_EXPORT void* get_module()
{
    static Php::Extension asio("asio", "1.0");

    //Interface Asio\Socket.
    Php::Interface socket("Asio\\Socket");
    socket.method("available");
    socket.method("atMark");
    socket.method("close");

    //Interface Asio\StreamSocket.
    Php::Interface stream_socket("Asio\\StreamSocket");
    stream_socket.extends(socket);
    stream_socket.method("read", {
        Php::ByVal("length", Php::Type::Numeric),
        Php::ByVal("read_some", Php::Type::Bool),
        Php::ByVal("callback", Php::Type::Callable),
        Php::ByVal("argument", Php::Type::Null, false),
    });
    stream_socket.method("write", {
        Php::ByVal("data", Php::Type::String),
        Php::ByVal("write_some", Php::Type::Bool),
        Php::ByVal("callback", Php::Type::Callable, false),
        Php::ByVal("argument", Php::Type::Null, false),
    });

    //Class Asio\Service.
    Php::Class<Service> service("Asio\\Service");

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
    service.method<&Service::addSignal>("addSignal", {
        Php::ByVal("callback", Php::Type::Callable),
        Php::ByVal("argument", Php::Type::Null, false),
    });
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
    timer.method<&Timer::defer>("defer", {
        Php::ByVal("interval", Php::Type::Numeric, false),
        Php::ByVal("callback", Php::Type::Callable, false),
        Php::ByVal("argument", Php::Type::Null, false)
    });
    timer.method<&Timer::cancel>("cancel");

    //Class Asio\TcpServer.
    Php::Class<TcpServer> tcp_server("Asio\\TcpServer", Php::Final);
    tcp_server.method<&TcpServer::accept>("accept", {
        Php::ByVal("callback", Php::Type::Callable, false),
        Php::ByVal("argument", Php::Type::Null, false)
    });
    tcp_server.method<&TcpServer::stop>("stop");

    //Class Asio\TcpSocket.
    Php::Class<TcpSocket> tcp_socket("Asio\\TcpSocket", Php::Final);
    tcp_socket.implements(stream_socket);
    tcp_socket.method<&TcpSocket::read>("read", {
        Php::ByVal("length", Php::Type::Numeric),
        Php::ByVal("read_some", Php::Type::Bool),
        Php::ByVal("callback", Php::Type::Callable),
        Php::ByVal("argument", Php::Type::Null, false),
    });
    tcp_socket.method<&TcpSocket::write>("write", {
        Php::ByVal("data", Php::Type::String),
        Php::ByVal("write_some", Php::Type::Bool),
        Php::ByVal("callback", Php::Type::Callable, false),
        Php::ByVal("argument", Php::Type::Null, false),
    });
    tcp_socket.method<&TcpSocket::available>("available");
    tcp_socket.method<&TcpSocket::atMark>("atMark");
    tcp_socket.method<&TcpSocket::close>("close");

    //Class Asio\Signal.
    Php::Class<Signal> signal("Asio\\Signal", Php::Final);
    signal.method<&Signal::add>("add");
    signal.method<&Signal::remove>("remove");
    signal.method<&Signal::clear>("clear");
    signal.method<&Signal::cancel>("cancel");

    asio.add(std::move(socket));
    asio.add(std::move(stream_socket));
    asio.add(std::move(service));
    asio.add(std::move(timer));
    asio.add(std::move(tcp_server));
    asio.add(std::move(tcp_socket));
    asio.add(std::move(signal));
    return asio;
}