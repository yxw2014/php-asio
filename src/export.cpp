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
    asio.add(std::move(socket));

    //Interface Asio\StreamSocket.
    Php::Interface stream_socket("Asio\\StreamSocket");
    stream_socket.extends(socket);
    stream_socket.method("read",
    {
        Php::ByVal("length", Php::Type::Numeric),
        Php::ByVal("read_some", Php::Type::Bool),
        Php::ByVal("callback", Php::Type::Callable),
        Php::ByVal("argument", Php::Type::Null, false),
    });
    stream_socket.method("write",
    {
        Php::ByVal("data", Php::Type::String),
        Php::ByVal("write_some", Php::Type::Bool),
        Php::ByVal("callback", Php::Type::Callable, false),
        Php::ByVal("argument", Php::Type::Null, false),
    });
    asio.add(std::move(stream_socket));

    //Interface Asio\Server.
    Php::Interface server("Asio\\Server");
    server.method("accept",
    {
        Php::ByVal("callback", Php::Type::Callable, false),
        Php::ByVal("argument", Php::Type::Null, false)
    });
    server.method("stop");
    asio.add(std::move(server));

    //Class Asio\Service.
    Php::Class<Service> service("Asio\\Service");
    service.method<&Service::add_timer>("addTimer");
    service.method<&Service::add_tcp_server>("addTcpServer",
    {
        Php::ByVal("address", Php::Type::String),
        Php::ByVal("port", Php::Type::Numeric),
    });
    service.method<&Service::add_unix_server>("addUnixServer",
    {
        Php::ByVal("path", Php::Type::String),
    });
    service.method<&Service::add_signal>("addSignal");
    service.method<&Service::run>("run");
    service.method<&Service::run_one>("runOne");
    service.method<&Service::poll>("poll");
    service.method<&Service::poll_one>("pollOne");
    service.method<&Service::stop>("stop");
    service.method<&Service::stopped>("stopped");
    service.method<&Service::reset>("reset");
    service.method<&Service::post>("post",
    {
        Php::ByVal("callback", Php::Type::Callable),
        Php::ByVal("argument", Php::Type::Null, false)
    });
    service.method<&Service::get_last_error>("lastError");
    asio.add(std::move(service));

    //Class Asio\Future.
    Php::Class<Future> future("Asio\\Future", Php::Final);
    asio.add(std::move(future));

    //Class Asio\Timer.
    Php::Class<Timer> timer("Asio\\Timer", Php::Final);
    timer.method<&Timer::wait>("wait",
    {
        Php::ByVal("interval", Php::Type::Numeric),
        Php::ByVal("callback", Php::Type::Callable, false),
        Php::ByVal("argument", Php::Type::Null, false)
    });
    timer.method<&Timer::cancel>("cancel");
    asio.add(std::move(timer));

    //Class Asio\TcpServer.
    Php::Class<TcpServer> tcp_server("Asio\\TcpServer", Php::Final);
    tcp_server.implements(server);
    tcp_server.method<&TcpServer::accept>("accept",
    {
        Php::ByVal("callback", Php::Type::Callable, false),
        Php::ByVal("argument", Php::Type::Null, false)
    });
    tcp_server.method<&TcpServer::stop>("stop");
    asio.add(std::move(tcp_server));

    //Class Asio\UnixServer.
    Php::Class<UnixServer> unix_server("Asio\\UnixServer", Php::Final);
    unix_server.implements(server);
    unix_server.method<&UnixServer::accept>("accept",
    {
        Php::ByVal("callback", Php::Type::Callable, false),
        Php::ByVal("argument", Php::Type::Null, false)
    });
    unix_server.method<&UnixServer::stop>("stop");
    asio.add(std::move(unix_server));

    //Class Asio\TcpSocket.
    Php::Class<TcpSocket> tcp_socket("Asio\\TcpSocket", Php::Final);
    tcp_socket.implements(stream_socket);
    tcp_socket.method<&TcpSocket::read>("read",
    {
        Php::ByVal("length", Php::Type::Numeric),
        Php::ByVal("read_some", Php::Type::Bool),
        Php::ByVal("callback", Php::Type::Callable, false),
        Php::ByVal("argument", Php::Type::Null, false),
    });
    tcp_socket.method<&TcpSocket::write>("write",
    {
        Php::ByVal("data", Php::Type::String),
        Php::ByVal("write_some", Php::Type::Bool),
        Php::ByVal("callback", Php::Type::Callable, false),
        Php::ByVal("argument", Php::Type::Null, false),
    });
    tcp_socket.method<&TcpSocket::available>("available");
    tcp_socket.method<&TcpSocket::at_mark>("atMark");
    tcp_socket.method<&TcpSocket::close>("close");
    asio.add(std::move(tcp_socket));

    //Class Asio\UnixSocket.
    Php::Class<UnixSocket> unix_socket("Asio\\UnixSocket", Php::Final);
    unix_socket.implements(stream_socket);
    unix_socket.method<&UnixSocket::read>("read",
    {
        Php::ByVal("length", Php::Type::Numeric),
        Php::ByVal("read_some", Php::Type::Bool),
        Php::ByVal("callback", Php::Type::Callable),
        Php::ByVal("argument", Php::Type::Null, false),
    });
    unix_socket.method<&UnixSocket::write>("write",
    {
        Php::ByVal("data", Php::Type::String),
        Php::ByVal("write_some", Php::Type::Bool),
        Php::ByVal("callback", Php::Type::Callable, false),
        Php::ByVal("argument", Php::Type::Null, false),
    });
    unix_socket.method<&UnixSocket::available>("available");
    unix_socket.method<&UnixSocket::at_mark>("atMark");
    unix_socket.method<&UnixSocket::close>("close");
    asio.add(std::move(unix_socket));

    //Class Asio\Signal.
    Php::Class<Signal> signal("Asio\\Signal", Php::Final);
    signal.method<&Signal::add>("add");
    signal.method<&Signal::wait>("wait",
    {
        Php::ByVal("callback", Php::Type::Callable, false),
        Php::ByVal("argument", Php::Type::Null, false)
    });
    signal.method<&Signal::remove>("remove");
    signal.method<&Signal::clear>("clear");
    signal.method<&Signal::cancel>("cancel");
    asio.add(std::move(signal));

    return asio;
}