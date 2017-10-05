#include "includes.hpp"

namespace Asio
{
    Base::Base(boost::asio::io_service& io_service) : _io_service(io_service) {}
}