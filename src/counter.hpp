#pragma once

class Counter
{
public:

    /**
     * Global counter.
     */
#if defined(PHP_ASIO_MT)
#include <atomic>
    static std::atomic_int64_t current;
#else
    static int64_t current;
#endif
};