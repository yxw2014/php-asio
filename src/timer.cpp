#include "includes.hpp"

void Timer::_defer()
{
    _timer->expires_from_now(boost::posix_time::milliseconds(_interval));
    _timer->async_wait(boost::bind(&Timer::_handler, this));
}

void Timer::_handler()
{
    _callback(_id, _argument);
    if (_persistent)
        _defer();
    else
        del(_service_id, _id);
}

Timer::Timer(
    IoService& io_service,
    int64_t interval,
    const Php::Value& argument,
    const Php::Value& callback,
    bool persistent
) : _interval(interval), _argument(argument), _callback(callback), _persistent(persistent), _id(Counter::increment(Counter::object_type::timer))
{
    _timer = new boost::asio::deadline_timer(io_service());
    auto service_id = io_service.getId();
    auto iterator = _timer_map.find(service_id);
    if (iterator == _timer_map.end())
    {
        std::map<int64_t, Timer *> new_map;
        new_map.insert(std::make_pair(_id, this));
        _timer_map.insert(make_pair(service_id, new_map));
    }
    else
        iterator->second.insert(std::make_pair(_id, this));
    _service_id = io_service.getId();
    _defer();
}

Timer::~Timer()
{
    _timer->cancel();
    delete _timer;
    Counter::erase(Counter::object_type::timer, _id);
}

int64_t Timer::getId() const
{
    return _id;
}

void Timer::del(int64_t service_id, int64_t timer_id)
{
    auto map = _timer_map[service_id];
    auto iterator = map.find(timer_id);
    if (iterator == map.end())
        throw Php::Exception("Timer does not exist.");
    delete iterator->second;
    map.erase(iterator);
}

void Timer::delAll(int64_t service_id)
{
    auto iterator = _timer_map.find(service_id);
    for (auto& timer : iterator->second)
    {
        delete timer.second;
        Counter::erase(Counter::object_type::timer, timer.first);
    }
    _timer_map.erase(iterator);
}

void Timer::cancel(int64_t service_id, int64_t timer_id)
{
    auto map = _timer_map[service_id];
    auto iterator = map.find(timer_id);
    if (iterator == map.end())
        throw Php::Exception("Timer does not exist.");
    iterator->second->_persistent = false;
}

std::map<int64_t, std::map<int64_t, Timer *>> Timer::_timer_map;