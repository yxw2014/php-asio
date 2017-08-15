#include "includes.hpp"

#if defined(PHP_ASIO_MT)
std::atomic_int64_t Counter::current = 0;
#else
int64_t Counter::current = 0;
#endif

std::set<int64_t> Counter::_service_set;

std::set<int64_t> Counter::_timer_set;


int64_t Counter::increment(object_type type)
{
    std::set<int64_t>* set = nullptr;
    switch (type)
    {
    case service:
        set = &_service_set;
        break;
    case timer:
        set = &_timer_set;
        break;
    }
    while (set->find(current++) != set->end());
    set->insert(current - 1);
    return current - 1;
}

void Counter::erase(object_type type, int64_t id)
{
    std::set<int64_t>* set = nullptr;
    switch (type)
    {
    case service:
        set = &_service_set;
        break;
    case timer:
        set = &_timer_set;
        break;
    }
    auto iterator = set->find(id);
    if (iterator == set->end())
        return;
    set->erase(iterator);
}