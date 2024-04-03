#include "Core/Timer.h"

Timer::Timer()
{
    m_sdlMilliSecond = 0;
    m_isPause = false;
    m_MilliSecond = 0;
    m_deltaMilliSecond = 0;
    m_pauseSDLMilliSecond = 0;
}

void Timer::updateTime()
{
    if (m_isPause) return;
    uint64_t now_sdlMilliSecond = SDL_GetTicks64();
    m_deltaMilliSecond = now_sdlMilliSecond - m_sdlMilliSecond;
    m_MilliSecond += m_deltaMilliSecond;
    m_sdlMilliSecond = now_sdlMilliSecond;
}

void Timer::pause()
{
    if (m_isPause) return;
    m_isPause = true;
    m_pauseSDLMilliSecond = SDL_GetTicks64();
}

void Timer::start()
{
    if (!m_isPause) return;
    m_isPause = false;
    uint64_t offset_ms = m_pauseSDLMilliSecond - m_sdlMilliSecond;
    m_sdlMilliSecond = SDL_GetTicks64() - offset_ms;
}

void Timer::reset()
{
    m_sdlMilliSecond = SDL_GetTicks64();
    m_MilliSecond = 0;
    m_deltaMilliSecond = 0;
}

Timer::~Timer()
{}