#include "Zombies/ZombieObject.h"

ZombieObject::ZombieObject(
    std::shared_ptr<AnimLoader> loader,
    const SDL_FPoint& init_point,
    const SDL_FRect& aabb,
    const SDL_FPoint& offset_shadow,
    float shadow_width,
    float shadow_height,
    int HP,
    ZombieState state
) :
    GameObject(aabb),
    m_HP(HP),
    m_state(state),
    m_referenceScreenPoint(init_point),
    m_offsetAABB(SDL_FPoint{ aabb.x - init_point.x, aabb.y - init_point.y }),
    m_offsetShadow(offset_shadow),
    m_animPlayer(loader, init_point),
    m_shadowRange(SDL_FRect{ init_point.x + offset_shadow.x, init_point.y + offset_shadow.y, shadow_width, shadow_height })
{
    m_shadow = GlobalVars::getInstance().textureRes.getTextureFrom("images/plantshadow.png");
}

ZombieObject::ZombieObject(
    std::shared_ptr<AnimLoader> loader,
    const SDL_FPoint& init_point,
    const SDL_FPoint& offset_abbb,
    float aabb_width,
    float aabb_height,
    const SDL_FPoint& offset_shadow,
    float shadow_width,
    float shadow_height,
    int HP,
    ZombieState state
) :
    GameObject(SDL_FRect{ init_point.x + offset_abbb.x, init_point.y + offset_abbb.y, aabb_width, aabb_height }),
    m_HP(HP),
    m_state(state),
    m_offsetAABB(offset_abbb),
    m_offsetShadow(offset_shadow),
    m_animPlayer(loader, init_point),
    m_shadowRange(SDL_FRect{ init_point.x + offset_shadow.x, init_point.y + offset_shadow.y, shadow_width, shadow_height })
{
    m_shadow = GlobalVars::getInstance().textureRes.getTextureFrom("images/plantshadow.png");
}

int ZombieObject::damage(int damege_num)
{
    m_HP -= damege_num;
    return 0;
}

int ZombieObject::showAABB()
{
    SDL_FRect aabb = m_aabb;
    aabb.x = GlobalVars::getInstance().camera.getRenderX(aabb.x);
    aabb.y = GlobalVars::getInstance().camera.getRenderY(aabb.y);
    return SDL_RenderDrawRectF(GlobalVars::getInstance().renderer, &aabb);
}

int ZombieObject::showShadow()
{
    SDL_FRect aabb = m_shadowRange;
    aabb.x = GlobalVars::getInstance().camera.getRenderX(aabb.x);
    aabb.y = GlobalVars::getInstance().camera.getRenderY(aabb.y);
    return SDL_RenderCopyF(GlobalVars::getInstance().renderer, m_shadow, NULL, &aabb);
}

ZombieObject::~ZombieObject()
{}