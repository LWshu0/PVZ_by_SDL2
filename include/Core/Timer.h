#ifndef TIMER_H
#define TIMER_H

#include "SDL.h"
/**
 * 当游戏暂停时, SDL 时间并不会停止
 * Timer 类用来记录游戏时间, 当游戏暂停时, Timer 的游戏时间也会暂停
 * 当 Timer 从暂停中恢复时, 它将从暂停的游戏时间开始继续计时
 */
class Timer
{
private:
    uint64_t m_sdlMilliSecond;          // 上一次更新时的 SDL 时间(毫秒)
    bool m_isPause;                     // 是否暂停中
    uint64_t m_MilliSecond;             // 上一次更新时的游戏时间(毫秒)
    uint64_t m_deltaMilliSecond;        // 距离上一次更新经过的游戏时间(毫秒)

    uint64_t m_pauseSDLMilliSecond;     // 暂停时的 SDL 时间(毫秒), 用与恢复到正确的游戏时间
public:
    Timer();

    void updateTime();

    void pause();

    void start();

    void reset();

    inline uint64_t getTime()
    {
        return m_MilliSecond;
    };

    inline uint64_t getDeltaTime()
    {
        return m_deltaMilliSecond;
    }

    ~Timer();
};

#endif