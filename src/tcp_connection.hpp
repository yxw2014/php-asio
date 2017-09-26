#pragma once

using boost::asio::ip::tcp;

/**
 * Wrapper for Boost.Asio stream socket.
 */
class TcpConnection : public Php::Base
{
    /**
     * Socket of current TCP connection.
     */
    tcp::socket _socket;

    /**
     * PHP wrapper for this object.
     */
    Php::Object* _wrapper;

    /**
     * Whether connection is marked as closed.
     */
    bool _closed = false;

    /**
     * Read handler for TCP connection.
     * @param error : Error code
     * @param length : Bytes transferred
     * @param callback : Handler callback
     * @param argument : Extra argument
     * @param buffer : Read buffer
     */
    void _read_handler(
        const boost::system::error_code& error,
        size_t length,
        const Php::Value& callback,
        const Php::Value& argument,
        std::vector<uint8_t>* buffer);

    /**
     * Write handler for TCP connection.
     * @param error : Error code
     * @param length : Bytes transferred
     * @param callback : Handler callback
     * @param argument : Extra argument
     * @param buffer : Write buffer
     */
    void _write_handler(
        const boost::system::error_code& error,
        size_t length,
        const Php::Value& callback,
        const Php::Value& argument,
        std::string* buffer);

    /**
     * The internal read method.
     * @param length : Maximum number fo bytes to read
     * @param read_some : If false, handler will not be called unless the required amount of bytes are recieved.
     * @param callback : Read handler callback
     * @param argument : Extra argument
     */
    void _read(int64_t length, bool read_some, const Php::Value& callback, const Php::Value& argument);

    /**
     * The internal write method.
     * @param data : Data to write to stream socket.
     * @param write_some : If false, handler will not be called unless all data is sent.
     * @param callback : Read handler callback
     * @param argument : Extra argument
     */
    void _write(const std::string& data, bool write_some, const Php::Value& callback, const Php::Value& argument);

public:

    /**
     * Constructor.
     * @param io_service : IO service of current TCP socket.
     */
    explicit TcpConnection(boost::asio::io_service& io_service);

    /**
     * Deleted default constructor.
     */
    TcpConnection() = delete;

    /**
     * Deleted copy constructor.
     */
    TcpConnection(const TcpConnection&) = delete;

    /**
     * Deleted copy assignment operator.
     */
    TcpConnection& operator=(const TcpConnection&) = delete;

    /**
     * Default destructor.
     */
    virtual ~TcpConnection();

    /**
     * Get reference of TCP socket.
     */
    tcp::socket& getSocket();

    /**
     * Read asynchronously from stream socket.
     */
    void read(Php::Parameters&);

    /**
     * Write asynchronously to stream socket.
     */
    void write(Php::Parameters&);

    /**
     * Determine the number of bytes available for reading.
     */
    Php::Value available() const;

    /**
     * Determine whether the socket is at the out-of-band data mark.
     */
    Php::Value atMark() const;

    /**
     * Close the TCP socket.
     */
    void close();

};
