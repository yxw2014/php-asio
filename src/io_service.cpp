#include "includes.hpp"

IoService::IoService() : _id(Counter::current++) {}

boost::asio::io_service& IoService::operator()()
{
	return _io_service;
}

int64_t IoService::getId() const
{
	return _id;
}
