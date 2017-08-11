#include "includes.hpp"

#if defined(PHP_ASIO_MT)
std::atomic_int64_t Counter::current = 0;
#else
int64_t Counter::current = 0;
#endif