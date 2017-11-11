/**
 * php-asio/base.cpp
 *
 * @author CismonX<admin@cismon.net>
 */

#include "base.hpp"

namespace Asio
{
    Base::Base(boost::asio::io_service& io_service) : io_service_(io_service) {}
}