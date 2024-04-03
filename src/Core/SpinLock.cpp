#include "Core/SpinLock.h"

void SpinLock::lock()
{
    while (flag.test_and_set(std::memory_order_acquire)) {}
}

void SpinLock::unlock()
{
    flag.clear(std::memory_order_release);
}