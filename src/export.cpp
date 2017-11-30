/**
 * php-asio/export.cpp
 *
 * @author CismonX<admin@cismon.net>
 */

#include "service.hpp"

using namespace Asio;

extern "C" PHPCPP_EXPORT void* get_module()
{
    static Php::Extension asio("asio", "1.0");

    // Interface Asio\Socket.
    Php::Interface socket("Asio\\Socket");
    socket.method("available", {
        Php::ByRef("ec", Php::Type::Null, false)
    });
    socket.method("atMark", {
        Php::ByRef("ec", Php::Type::Null, false)
    });
    socket.method("close");
    asio.add(std::move(socket));

    // Interface Asio\StreamSocket.
    Php::Interface stream_socket("Asio\\StreamSocket");
    stream_socket.extends(socket);
    stream_socket.method("read", {
        Php::ByVal("length", Php::Type::Numeric),
        Php::ByVal("read_some", Php::Type::Bool, false),
        Php::ByVal("callback", Php::Type::Callable, false),
        Php::ByVal("argument", Php::Type::Null, false),
    });
    stream_socket.method("write", {
        Php::ByVal("data", Php::Type::String),
        Php::ByVal("write_some", Php::Type::Bool, false),
        Php::ByVal("callback", Php::Type::Callable, false),
        Php::ByVal("argument", Php::Type::Null, false),
    });
    asio.add(std::move(stream_socket));

    // Interface Asio\InetSocket.
    Php::Interface inet_socket("Asio\\InetSocket");
    inet_socket.extends(socket);
    inet_socket.method("open", {
        Php::ByVal("use_ipv6", Php::Type::Bool)
    });
    inet_socket.method("bind", {
        Php::ByVal("address", Php::Type::String),
        Php::ByVal("port", Php::Type::Numeric)
    });
    inet_socket.method("remoteAddr");
    inet_socket.method("remotePort");
    asio.add(std::move(inet_socket));

    // Interface Asio\LocalSocket.
    Php::Interface local_socket("Asio\\LocalSocket");
    local_socket.extends(socket);
    local_socket.method("open");
    local_socket.method("bind", {
        Php::ByVal("path", Php::Type::String)
    });
    local_socket.method("remotePath");
    asio.add(std::move(local_socket));

    // Interface Asio\Acceptor.
    Php::Interface acceptor("Asio\\Acceptor");
    acceptor.method("listen", {
        Php::ByVal("backlog", Php::Type::Numeric, false)
    });
    acceptor.method("accept", {
        Php::ByVal("callback", Php::Type::Callable, false),
        Php::ByVal("argument", Php::Type::Null, false)
    });
    acceptor.method("stop");
    asio.add(std::move(acceptor));

    // Interface Asii\Resolver.
    Php::Interface resolver("Asio\\Resolver");
    resolver.method("resolve", {
        Php::ByVal("host", Php::Type::String),
        Php::ByVal("service", Php::Type::String, false),
        Php::ByVal("callback", Php::Type::Callable, false),
        Php::ByVal("argument", Php::Type::Null, false)
    });
    resolver.method("cancel");
    asio.add(std::move(resolver));

    // Class Asio\Service.
    Php::Class<Service> service("Asio\\Service");
    service.method<&Service::add_timer>("addTimer");
    service.method<&Service::add_tcp_acceptor>("addTcpAcceptor");
    service.method<&Service::add_unix_acceptor>("addUnixAcceptor");
    service.method<&Service::add_tcp_resolver>("addTcpResolver");
    service.method<&Service::add_udp_resolver>("addUdpResolver");
    service.method<&Service::add_tcp_socket>("addTcpSocket");
    service.method<&Service::add_unix_socket>("addUnixSocket");
    service.method<&Service::add_signal>("addSignal");
    service.method<&Service::run>("run", {
        Php::ByRef("ec", Php::Type::Null, false)
    });
    service.method<&Service::run_one>("runOne", {
        Php::ByRef("ec", Php::Type::Null, false)
    });
    service.method<&Service::poll>("poll", {
        Php::ByRef("ec", Php::Type::Null, false)
    });
    service.method<&Service::poll_one>("pollOne", {
        Php::ByRef("ec", Php::Type::Null, false)
    });
    service.method<&Service::stop>("stop");
    service.method<&Service::stopped>("stopped");
    service.method<&Service::reset>("reset");
    service.method<&Service::post>("post", {
        Php::ByVal("callback", Php::Type::Callable),
        Php::ByVal("argument", Php::Type::Null, false)
    });
    service.method<&Service::dispatch>("dispatch", {
        Php::ByVal("callback", Php::Type::Callable),
        Php::ByVal("argument", Php::Type::Null, false)
    });
    service.method<&Service::notify_fork>("notifyFork", {
        Php::ByVal("is_parent", Php::Type::Bool, false)
    });
    service.method<&Future::get_last_error>("lastError");
    asio.add(std::move(service));

    // Class Asio\Future.
    Php::Class<Future> future("Asio\\Future", Php::Final);
    asio.add(std::move(future));

    // Class Asio\Timer.
    Php::Class<Timer> timer("Asio\\Timer", Php::Final);
    timer.method<&Timer::expire>("expire", {
        Php::ByVal("time", Php::Type::Numeric),
        Php::ByVal("use_timestamp", Php::Type::Bool, false)
    });
    timer.method<&Timer::wait>("wait", {
        Php::ByVal("callback", Php::Type::Callable, false),
        Php::ByVal("argument", Php::Type::Null, false)
    });
    timer.method<&Timer::cancel>("cancel");
    asio.add(std::move(timer));

    // Class Asio\TcpAcceptor.
    Php::Class<TcpAcceptor> tcp_acceptor("Asio\\TcpAcceptor", Php::Final);
    tcp_acceptor.implements(acceptor);
    tcp_acceptor.method<&TcpAcceptor::open_inet>("open", {
        Php::ByVal("use_ipv6", Php::Type::Bool)
    });
    tcp_acceptor.method<&TcpAcceptor::bind>("bind", {
        Php::ByVal("address", Php::Type::String),
        Php::ByVal("port", Php::Type::Numeric)
    });
    tcp_acceptor.method<&TcpAcceptor::listen>("listen", {
        Php::ByVal("backlog", Php::Type::Numeric, false)
    });
    tcp_acceptor.method<&TcpAcceptor::accept>("accept", {
        Php::ByVal("callback", Php::Type::Callable, false),
        Php::ByVal("argument", Php::Type::Null, false)
    });
    tcp_acceptor.method<&TcpAcceptor::stop>("stop");
    asio.add(std::move(tcp_acceptor));

    // Class Asio\UnixAcceptor.
    Php::Class<UnixAcceptor> unix_acceptor("Asio\\UnixAcceptor", Php::Final);
    unix_acceptor.implements(acceptor);
    unix_acceptor.method<&UnixAcceptor::open_local>("open");
    unix_acceptor.method<&UnixAcceptor::bind>("bind", {
        Php::ByVal("path", Php::Type::String)
    });
    unix_acceptor.method<&UnixAcceptor::listen>("listen", {
        Php::ByVal("backlog", Php::Type::Numeric, false)
    });
    unix_acceptor.method<&UnixAcceptor::accept>("accept", {
        Php::ByVal("callback", Php::Type::Callable, false),
        Php::ByVal("argument", Php::Type::Null, false)
    });
    unix_acceptor.method<&UnixAcceptor::stop>("stop");
    asio.add(std::move(unix_acceptor));

    // Class Asio\TcpSocket.
    Php::Class<TcpSocket> tcp_socket("Asio\\TcpSocket", Php::Final);
    tcp_socket.implements(stream_socket);
    tcp_socket.implements(inet_socket);
    tcp_socket.method<&TcpSocket::open_inet>("open", {
        Php::ByVal("use_ipv6", Php::Type::Bool)
    });
    tcp_socket.method<&TcpSocket::connect>("connect", {
        Php::ByVal("address", Php::Type::String),
        Php::ByVal("port", Php::Type::Numeric),
        Php::ByVal("callback", Php::Type::Callable, false),
        Php::ByVal("argument", Php::Type::Null, false)
    });
    tcp_socket.method<&TcpSocket::read>("read", {
        Php::ByVal("length", Php::Type::Numeric),
        Php::ByVal("read_some", Php::Type::Bool, false),
        Php::ByVal("callback", Php::Type::Callable, false),
        Php::ByVal("argument", Php::Type::Null, false),
    });
    tcp_socket.method<&TcpSocket::write>("write", {
        Php::ByVal("data", Php::Type::String),
        Php::ByVal("write_some", Php::Type::Bool, false),
        Php::ByVal("callback", Php::Type::Callable, false),
        Php::ByVal("argument", Php::Type::Null, false),
    });
    tcp_socket.method<&TcpSocket::bind>("bind", {
        Php::ByVal("address", Php::Type::String),
        Php::ByVal("port", Php::Type::Numeric)
    });
    tcp_socket.method<&TcpSocket::remote_addr>("remoteAddr");
    tcp_socket.method<&TcpSocket::remote_port>("remotePort");
    tcp_socket.method<&TcpSocket::available>("available", {
        Php::ByRef("ec", Php::Type::Null, false)
    });
    tcp_socket.method<&TcpSocket::at_mark>("atMark", {
        Php::ByRef("ec", Php::Type::Null, false)
    });
    tcp_socket.method<&TcpSocket::close>("close");
    asio.add(std::move(tcp_socket));

    // Class Asio\UnixSocket.
    Php::Class<UnixSocket> unix_socket("Asio\\UnixSocket", Php::Final);
    unix_socket.implements(stream_socket);
    unix_socket.implements(local_socket);
    unix_socket.method<&UnixSocket::open_local>("open");
    unix_socket.method<&UnixSocket::connect>("connect", {
        Php::ByVal("path", Php::Type::String),
        Php::ByVal("callback", Php::Type::Callable, false),
        Php::ByVal("argument", Php::Type::Null, false)
    });
    unix_socket.method<&UnixSocket::read>("read", {
        Php::ByVal("length", Php::Type::Numeric),
        Php::ByVal("read_some", Php::Type::Bool, false),
        Php::ByVal("callback", Php::Type::Callable, false),
        Php::ByVal("argument", Php::Type::Null, false)
    });
    unix_socket.method<&UnixSocket::write>("write", {
        Php::ByVal("data", Php::Type::String),
        Php::ByVal("write_some", Php::Type::Bool, false),
        Php::ByVal("callback", Php::Type::Callable, false),
        Php::ByVal("argument", Php::Type::Null, false)
    });
    unix_socket.method<&UnixSocket::bind>("bind", {
        Php::ByVal("path", Php::Type::String)
    });
    unix_socket.method<&UnixSocket::remote_path>("remotePath");
    unix_socket.method<&UnixSocket::available>("available", {
        Php::ByRef("ec", Php::Type::Null, false)
    });
    unix_socket.method<&UnixSocket::at_mark>("atMark", {
        Php::ByRef("ec", Php::Type::Null, false)
    });
    unix_socket.method<&UnixSocket::close>("close");
    asio.add(std::move(unix_socket));

    // Class Asio\TcpResolver.
    Php::Class<TcpResolver> tcp_resolver("Asio\\TcpResolver", Php::Final);
    tcp_resolver.implements(resolver);
    tcp_resolver.method<&TcpResolver::resolve>("resolve", {
        Php::ByVal("host", Php::Type::String),
        Php::ByVal("service", Php::Type::String, false),
        Php::ByVal("callback", Php::Type::Callable, false),
        Php::ByVal("argument", Php::Type::Null, false)
    });
    tcp_resolver.method<&TcpResolver::cancel>("cancel");
    asio.add(std::move(tcp_resolver));

    // Class Asio\UdpResolver.
    Php::Class<UdpResolver> udp_resolver("Asio\\UdpResolver", Php::Final);
    udp_resolver.implements(resolver);
    udp_resolver.method<&UdpResolver::resolve>("resolve", {
        Php::ByVal("host", Php::Type::String),
        Php::ByVal("service", Php::Type::String, false),
        Php::ByVal("callback", Php::Type::Callable, false),
        Php::ByVal("argument", Php::Type::Null, false)
    });
    udp_resolver.method<&UdpResolver::cancel>("cancel");
    asio.add(std::move(udp_resolver));

    // Class Asio\Signal.
    Php::Class<Signal> signal("Asio\\Signal", Php::Final);
    signal.method<&Signal::add>("add");
    signal.method<&Signal::wait>("wait", {
        Php::ByVal("callback", Php::Type::Callable, false),
        Php::ByVal("argument", Php::Type::Null, false)
    });
    signal.method<&Signal::remove>("remove");
    signal.method<&Signal::clear>("clear");
    signal.method<&Signal::cancel>("cancel");
    asio.add(std::move(signal));

    return asio;
}
