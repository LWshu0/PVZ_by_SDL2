#ifndef SPINLOCK_H
#define SPINLOCK_H

#include <atomic>

class SpinLock {
public:
    SpinLock() : flag(ATOMIC_FLAG_INIT) {}

    void lock();

    void unlock();

private:
    std::atomic_flag flag;
};

#endif