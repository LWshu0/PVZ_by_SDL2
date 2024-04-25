#include "Core/ObjectMotion.h"

int MotionSpeed::step(GameObject* object, uint64_t delta_ms)
{
    if (nullptr == object) return -1;
    float delta_x = m_speedX * static_cast<float>(delta_ms) / 1000.0f;
    float delta_y = m_speedY * static_cast<float>(delta_ms) / 1000.0f;
    object->m_aabb.x += delta_x;
    object->m_aabb.y += delta_y;
    return 0;
}

int MotionSpeedLimitTime::step(GameObject* object, uint64_t delta_ms)
{
    if (nullptr == object) return -1;
    if (m_lifetime < delta_ms) return 0;
    m_lifetime -= delta_ms;
    float delta_x = m_speedX * static_cast<float>(delta_ms) / 1000.0f;
    float delta_y = m_speedY * static_cast<float>(delta_ms) / 1000.0f;
    object->m_aabb.x += delta_x;
    object->m_aabb.y += delta_y;
    return 0;
}