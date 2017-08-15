#pragma once

class IoService
{
    /**
     * Unique id.
     */
    int64_t _id;

    /**
     * Boost.Asio io_service instance.
     */
    boost::asio::io_service _io_service;

public:

    /**
     * Constructor.
     */
    IoService();

    /**
     * Destructor.
     */
    ~IoService();

    /**
     * Get refernce of io_service.
     */
    boost::asio::io_service& operator()();

    /**
     * Get id of io_service.
     */
    int64_t getId() const;
};