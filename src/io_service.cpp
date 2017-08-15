#include "includes.hpp"

IoService::IoService() : _id(Counter::increment(Counter::object_type::service)) {}

IoService::~IoService()
{
    Counter::erase(Counter::object_type::service, _id);
}

boost::asio::io_service& IoService::operator()()
{
    return _io_service;
}

int64_t IoService::getId() const
{
    return _id;
}