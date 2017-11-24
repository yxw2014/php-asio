/**
 * php-asio/include/service.hpp
 *
 * @author CismonX<admin@cismon.net>
 */

#pragma once

#include "common.hpp"
#include "timer.hpp"
#include "acceptor.hpp"
#include "resolver.hpp"
#include "signal.hpp"

namespace Asio
{
    /**
     * Wrapper for Boost.Asio IO service.
     * Provide access to instantiation of IO objects.
     */
    class Service : public Php::Base
    {
        /**
         * The io_service of all IO objects in current instance.
         */
        boost::asio::io_service io_service_;

    public:
        /**
         * Default constructor.
         */
        Service() = default;

        /**
         * Deleted copy constructor.
         */
        Service(const Service&) = delete;

        /**
         * Default destructor.
         */
        virtual ~Service() = default;

        /**
         * Deleted copy assignment operator.
         */
        Service& operator=(const Service&) = delete;

        /**
         * Add a new timer.
         */
        Php::Value add_timer()
        {
            return new Timer(io_service_);
        }

        /**
         * Add a new TCP acceptor.
         */
        Php::Value add_tcp_acceptor()
        {
            return new TcpAcceptor(io_service_);
        }

        /**
         * Add a new UNIX domain socket acceptor (SOCK_STREAM).
         */
        Php::Value add_unix_acceptor()
        {
            return new UnixAcceptor(io_service_);
        }

        /**
         * Add a TCP resolver.
         */
        Php::Value add_tcp_resolver(Php::Parameters& params)
        {
            return new TcpResolver(io_service_);
        }

        /**
         * Add a UDP resolver.
         */
        Php::Value add_udp_resolver(Php::Parameters& params)
        {
            return new UdpResolver(io_service_);
        }

        /**
         * Add a new TCP socket.
         */
        Php::Value add_tcp_socket()
        {
            return new TcpSocket(io_service_);
        }

        /**
         * Add a new UNIX domain socket (SOCK_STREAM).
         */
        Php::Value add_unix_socket()
        {
            return new UnixSocket(io_service_);
        }

        /**
         * Add new signal handler.
         */
        Php::Value add_signal()
        {
            return new Signal(io_service_);
        }

        /**
         * Start event loop in block mode.
         */
        Php::Value run(Php::Parameters& params)
        {
            boost::system::error_code ec;
            auto handler_count = io_service_.run(ec);
            if (params.size())
                params[0] = ec.value();
            return static_cast<int64_t>(handler_count);
        }

        /**
         * Excecute at most one handler within the event loop in block mode.
         */
        Php::Value run_one(Php::Parameters& params)
        {
            boost::system::error_code ec;
            auto handler_count = io_service_.run_one(ec);
            if (params.size())
                params[0] = ec.value();
            return static_cast<int64_t>(handler_count);
        }

        /**
         * Start event loop in non-block mode.
         */
        Php::Value poll(Php::Parameters& params)
        {
            boost::system::error_code ec;
            auto handler_count = io_service_.poll(ec);
            if (params.size())
                params[0] = ec.value();
            return static_cast<int64_t>(handler_count);
        }

        /**
         * Excecute at most one handler within the event loop in non-block mode.
         */
        Php::Value poll_one(Php::Parameters& params)
        {
            boost::system::error_code ec;
            auto handler_count = io_service_.poll_one(ec);
            if (params.size())
                params[0] = ec.value();
            return static_cast<int64_t>(handler_count);
        }

        /**
         * Cancel all pending handlers within the event loop.
         */
        void stop()
        {
            io_service_.stop();
        }

        /**
         * Reset the io_service before start it again.
         */
        void reset()
        {
            io_service_.reset();
        }

        /**
         * Check whether the event loop has stopped.
         */
        Php::Value stopped() const
        {
            return io_service_.stopped();
        }

        /**
         * Execute a given callback with argument at the next tick.
         */
        void post(Php::Parameters& params)
        {
            auto callback = params[0];
            auto argument = params.size() == 1 ? Php::Value() : params[1];
            io_service_.post([callback, argument]()
            {
                Future::coroutine(callback(argument));
            });
        }

        /**
         * Get io_service by reference.
         * 
         * Can be used when working on another extension based on Boost.Asio.
         */
        boost::asio::io_service& get_io_service()
        {
            return io_service_;
        }

    };
}
