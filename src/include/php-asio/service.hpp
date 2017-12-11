/**
 * php-asio/include/service.hpp
 *
 * @author CismonX<admin@cismon.net>
 */

#pragma once

#include "common.hpp"
#include "strand.hpp"
#include "timer.hpp"
#include "acceptor.hpp"
#include "resolver.hpp"
#include "signal.hpp"

namespace Asio
{
    /**
     * Wrapper for Boost.Asio io_service.
     * Provide access to instantiation of I/O objects.
     */
    class Service : public Php::Base
    {
        /**
         * The io_service of all I/O objects in current instance.
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
         * Add a new UDP socket.
         */
        Php::Value add_udp_socket()
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
         * Add a new UNIX domain socket (SOCK_DGRAM).
         */
        Php::Value add_udg_socket()
        {
            return new UdgSocket(io_service_);
        }

        /**
         * Add new signal set.
         */
        Php::Value add_signal()
        {
            return new Signal(io_service_);
        }

#ifdef ENABLE_STRAND
        Php::Value add_strand()
        {
            return new Strand(io_service_);
        }
#endif // ENABLE_STRAND

        /**
         * Run the event loop until stopped or no more work.
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
         * Run until stopped or one operation is performed.
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
         * Poll for operations without blocking.
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
         * Poll for one operation without blocking.
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
         * Stop the event processing loop.
         */
        void stop()
        {
            io_service_.stop();
        }

        /**
         * Reset in preparation for a subsequent run invocation.
         */
        void reset()
        {
            io_service_.reset();
        }

        /**
         * Determine whether the io_service is stopped.
         */
        Php::Value stopped() const
        {
            return io_service_.stopped();
        }

        /**
         * Request invocation of the given handler and return immediately.
         */
        void post(Php::Parameters& params)
        {
            auto callback = params[0];
            auto argument = params.size() == 1 ? Php::Value() : params[1];
            io_service_.post([callback, argument]()
            {
                CORO_REGISTER(callback(argument));
            });
        }

        /**
         * Request invocation of the given handler.
         */
        void dispatch(Php::Parameters& params)
        {
            auto callback = params[0];
            auto argument = params.size() == 1 ? Php::Value() : params[1];
            io_service_.dispatch([callback, argument]()
            {
                CORO_REGISTER(callback(argument));
            });
        }

        /**
         * Notify all services of a fork event.
         */
        Php::Value notify_fork(Php::Parameters& params)
        {
            try {
                io_service_.notify_fork(params.size() ? (params[0].boolValue() ?
                    boost::asio::io_service::fork_parent : boost::asio::io_service::fork_child) : boost::asio::io_service::fork_prepare);
            } catch (const boost::system::system_error& err) {
                return err.code().value();
            }
            return 0;
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
