#pragma once

namespace Asio
{
    /**
     * Wrapper for Boost.Asio TCP acceptor.
     * Provide TCP services.
     */
    class TcpServer : public Base
    {

        /**
         * Acceptor for this server.
         */
        tcp::acceptor* _acceptor;

        /**
         * Argument to be passed to acceptor callback.
         */
        Php::Value _argument;

        /**
         * Timer callback.
         */
        Php::Value _callback;

        /**
         * Whether the server start accepting once created.
         */
        bool _auto_accept;

        /**
         * Whether the server is marked as stopped.
         */
        bool _stopped = false;

        /**
         * Boolean flag for execution context.
         */
        bool _context_flag = false;

        /**
         * Async accept.
         */
        void _accept();

        /**
         * Accept handler.
         * @param error : Error code
         * @param socket : Client connection
         */
        void _handler(const boost::system::error_code& error, const TcpSocket* socket);

    public:

        /**
         * Constructor.
         * @param io_service : IO service for current TCP server.
         * @param address : Address which the server will bind to.
         * @param port : Port which the server will bind to.
         * @param argument : Extra argument to be passed to acceptor callback.
         * @param callback : Acceptor callback.
         */
        explicit TcpServer(
            boost::asio::io_service& io_service,
            const std::string& address,
            unsigned short port,
            bool auto_accept,
            const Php::Value& argument,
            const Php::Value& callback);

        /**
         * Destructor.
         */
        virtual ~TcpServer();

        /**
         * Accept incoming client connection once.
         */
        void accept(Php::Parameters&);

        /**
         * Stop TCP server.
         */
        void stop();
    };

}