#pragma once

class Counter
{
    /**
     * Global counter.
     */
#if defined(PHP_ASIO_MT)
#include <atomic>
    static std::atomic_int64_t current;
#else
    static int64_t current;
#endif

public:

    /**
     * Enum of all object types.
     */
    enum object_type : char
    {
        service, timer
    };

    /**
     * Set of io services.
     */
    static std::set<int64_t> _service_set;

    /**
     * Set of io objects.
     */
    static std::set<int64_t> _timer_set;

    /**
     * Increment the global counter and get a unique integer as id.
     * @param type : Type of object
     */
    static int64_t increment(object_type type);

    /**
     * Erase an object from the counter.
     * @param type : Type of object
     * @param id : ID of object.
     */
    static void erase(object_type type, int64_t id);
};