#include "Plants/PlantObject.h"

PlantObject::PlantObject(
    std::shared_ptr<AnimLoader> loader,
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
    GameObject(aabb),
    m_HP(HP),
    m_state(state),
    m_reloadMilliSecond(reload_ms),
    m_windUpDuration(windup_duration),
    m_nextAttackAnimMilliSecond(0),
    m_nextFireMilliSecond(0),
    m_offsetAABB(SDL_FPoint{ aabb.x - init_point.x, aabb.y - init_point.y }),
    m_animPlayer(loader, init_point),
    m_offsetShadow(offset_shadow),
    m_shadowRange(SDL_FRect{ init_point.x + offset_shadow.x, init_point.y + offset_shadow.y, shadow_width, shadow_height })
{
    m_shadow = GlobalVars::getInstance().textureRes.getTextureFrom("images/plantshadow.png");
}

PlantObject::PlantObject(
    std::shared_ptr<AnimLoader> loader,
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
    GameObject(SDL_FRect{ init_point.x + offset_abbb.x, init_point.y + offset_abbb.y, aabb_width, aabb_height }),
    m_HP(HP),
    m_state(state),
    m_reloadMilliSecond(reload_ms),
    m_windUpDuration(windup_duration),
    m_nextAttackAnimMilliSecond(0),
    m_nextFireMilliSecond(0),
    m_offsetAABB(offset_abbb),
    m_animPlayer(loader, init_point),
    m_offsetShadow(offset_shadow),
    m_shadowRange(SDL_FRect{ init_point.x + offset_shadow.x, init_point.y + offset_shadow.y, shadow_width, shadow_height })
{
    m_shadow = GlobalVars::getInstance().textureRes.getTextureFrom("images/plantshadow.png");
}

void PlantObject::setObjectPosition(const SDL_FPoint& real_point)
{
    m_animPlayer.setPlayPosition(real_point);
    m_aabb.x = real_point.x + m_offsetAABB.x;
    m_aabb.y = real_point.y + m_offsetAABB.y;
    m_shadowRange.x = real_point.x + m_offsetShadow.x;
    m_shadowRange.y = real_point.y + m_offsetShadow.y;
}

void PlantObject::damage(int damege_num)
{
    m_HP -= damege_num;
}

int PlantObject::showAABB()
{
    SDL_FRect aabb = m_aabb;
    aabb.x = GlobalVars::getInstance().camera.getRenderX(aabb.x);
    aabb.y = GlobalVars::getInstance().camera.getRenderY(aabb.y);
    return SDL_RenderDrawRectF(GlobalVars::getInstance().renderer, &aabb);
}

int PlantObject::showShadow()
{
    SDL_FRect aabb = m_shadowRange;
    aabb.x = GlobalVars::getInstance().camera.getRenderX(aabb.x);
    aabb.y = GlobalVars::getInstance().camera.getRenderY(aabb.y);
    return SDL_RenderCopyF(GlobalVars::getInstance().renderer, m_shadow, NULL, &aabb);
}