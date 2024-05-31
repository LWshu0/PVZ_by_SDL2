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

int MotionAccLimitTime::step(GameObject* object, uint64_t delta_ms)
{
    if (nullptr == object) return -1;
    if (m_lifetime < delta_ms) return 0;
    m_lifetime -= delta_ms;
    float delta_s = static_cast<float>(delta_ms) / 1000.0f;
    m_speedX += m_accX * delta_s;
    m_speedY += m_accY * delta_s;
    object->m_aabb.x += m_speedX * delta_s;
    object->m_aabb.y += m_speedY * delta_s;
    return 0;
}

int MotionTargetPoint::step(GameObject* object, uint64_t delta_ms)
{
    if (nullptr == object) return -1;
    float delta_s = static_cast<float>(delta_ms) / 1000.0f;
    float delta_x = (m_targetX - object->m_aabb.x);
    float delta_y = (m_targetY - object->m_aabb.y);
    float dis = sqrt(delta_x * delta_x + delta_y * delta_y);
    float dire_x = delta_x / dis * m_speed * delta_s;
    float dire_y = delta_y / dis * m_speed * delta_s;

    if (dis < delta_s * m_speed)
    {
        object->m_aabb.x = m_targetX;
        object->m_aabb.y = m_targetY;
        return -1;
    }
    else
    {
        object->m_aabb.x += dire_x;
        object->m_aabb.y += dire_y;
    }
    return 0;
}