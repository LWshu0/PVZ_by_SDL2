#include "Plants/PlantObject.h"

PlantObject::PlantObject(
    std::shared_ptr<AnimLoader> loader,
    std::shared_ptr<Camera> camera,
    const SDL_FPoint& init_point,
    const SDL_FRect& aabb,
    const SDL_FPoint& offset_shadow,
    float shadow_width,
    float shadow_height,
    int HP,
    PlantState state,
    uint64_t reload_ms,
    uint64_t windup_duration
) :
    AnimPlayer(loader, camera, init_point),
    GameObject(aabb),
    m_HP(HP),
    m_state(state),
    m_reloadMilliSecond(reload_ms),
    m_windUpDuration(windup_duration),
    m_nextAttackAnimMilliSecond(0),
    m_nextFireMilliSecond(0),
    m_offsetAABB(SDL_FPoint{ aabb.x - init_point.x, aabb.y - init_point.y }),
    m_offsetShadow(offset_shadow),
    m_shadowRange(SDL_FRect{ init_point.x + offset_shadow.x, init_point.y + offset_shadow.y, shadow_width, shadow_height })
{
    m_shadow = m_loader->m_imageRes->getTextureFrom("images/plantshadow.png");
}

PlantObject::PlantObject(
    std::shared_ptr<AnimLoader> loader,
    std::shared_ptr<Camera> camera,
    const SDL_FPoint& init_point,
    const SDL_FPoint& offset_abbb,
    float aabb_width,
    float aabb_height,
    const SDL_FPoint& offset_shadow,
    float shadow_width,
    float shadow_height,
    int HP,
    PlantState state,
    int reload_ms,
    uint64_t windup_duration
) :
    AnimPlayer(loader, camera, init_point),
    GameObject(SDL_FRect{ init_point.x + offset_abbb.x, init_point.y + offset_abbb.y, aabb_width, aabb_height }),
    m_HP(HP),
    m_state(state),
    m_reloadMilliSecond(reload_ms),
    m_windUpDuration(windup_duration),
    m_nextAttackAnimMilliSecond(0),
    m_nextFireMilliSecond(0),
    m_offsetAABB(offset_abbb),
    m_offsetShadow(offset_shadow),
    m_shadowRange(SDL_FRect{ init_point.x + offset_shadow.x, init_point.y + offset_shadow.y, shadow_width, shadow_height })
{
    m_shadow = m_loader->m_imageRes->getTextureFrom("images/plantshadow.png");
}

void PlantObject::setObjectPosition(const SDL_FPoint& real_point)
{
    m_realtimeScreenPoint = real_point;
    m_aabb.x = m_realtimeScreenPoint.x + m_offsetAABB.x;
    m_aabb.y = m_realtimeScreenPoint.y + m_offsetAABB.y;
    m_shadowRange.x = m_realtimeScreenPoint.x + m_offsetShadow.x;
    m_shadowRange.y = m_realtimeScreenPoint.y + m_offsetShadow.y;
}

int PlantObject::damage(int damege_num)
{
    m_HP -= damege_num;
    if (m_HP <= 0)
    {
        changeDamageState(DamageState::R_Death);
    }
    return 0;
}

int PlantObject::changeToStatic()
{
    changeAnimState(AnimState::R_IDLE);
    restartTrack();
    return 0;
}

int PlantObject::getAnimRange(float& width, float& height)
{
    float left = 99999.0f, right = -99999.0f, top = 99999.0f, bottom = -99999.0f;   // big enough
    for (auto track_idx : m_playingTrack)
    {
        int frame_idx = m_trackPlayRecord[track_idx].m_playingFrameIdx;
        for (int i = 0;i < 4;i++)
        {
            float temp_x = m_loader->m_tracks[track_idx].m_frames[frame_idx].m_point[i].x;
            float temp_y = m_loader->m_tracks[track_idx].m_frames[frame_idx].m_point[i].y;
            if (temp_x < left) left = temp_x;
            else if (temp_x > right) right = temp_x;
            if (temp_y < top) top = temp_y;
            else if (temp_y > bottom) bottom = temp_y;
        }
    }
    width = right - left + 1;
    height = bottom - top + 1;
    setPlayPosition(SDL_FPoint{ -left, -top });
    return 0;
}

int PlantObject::showAABB()
{
    SDL_FRect aabb = m_aabb;
    aabb.x = m_camera->getRenderX(aabb.x);
    aabb.y = m_camera->getRenderY(aabb.y);
    return SDL_RenderDrawRectF(m_loader->m_renderer, &aabb);
}

int PlantObject::showShadow()
{
    SDL_FRect aabb = m_shadowRange;
    aabb.x = m_camera->getRenderX(aabb.x);
    aabb.y = m_camera->getRenderY(aabb.y);
    return SDL_RenderCopyF(m_loader->m_renderer, m_shadow, NULL, &aabb);
}