#include "Zombies/ZombieObject.h"

ZombieObject::ZombieObject(
    std::shared_ptr<AnimLoader> loader,
    std::shared_ptr<Camera> camera,
    const SDL_FPoint& init_point,
    const SDL_FRect& aabb,
    const SDL_FPoint& offset_shadow,
    float shadow_width,
    float shadow_height,
    int HP,
    ZombieState state
) :
    AnimPlayer(loader, camera, init_point),
    GameObject(aabb),
    m_HP(HP),
    m_state(state),
    m_offsetAABB(SDL_FPoint{ aabb.x - init_point.x, aabb.y - init_point.y }),
    m_offsetShadow(offset_shadow),
    m_shadowRange(SDL_FRect{ init_point.x + offset_shadow.x, init_point.y + offset_shadow.y, shadow_width, shadow_height })
{
    m_shadow = m_loader->m_imageRes->getTextureFrom("images/plantshadow.png");
}

ZombieObject::ZombieObject(
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
    ZombieState state
) :
    AnimPlayer(loader, camera, init_point),
    GameObject(SDL_FRect{ init_point.x + offset_abbb.x, init_point.y + offset_abbb.y, aabb_width, aabb_height }),
    m_HP(HP),
    m_state(state),
    m_offsetAABB(offset_abbb),
    m_offsetShadow(offset_shadow),
    m_shadowRange(SDL_FRect{ init_point.x + offset_shadow.x, init_point.y + offset_shadow.y, shadow_width, shadow_height })
{
    m_shadow = m_loader->m_imageRes->getTextureFrom("images/plantshadow.png");
}

int ZombieObject::damage(int damege_num)
{
    m_HP -= damege_num;
    if (m_HP <= 0)
    {
        changeAnimState(AnimState::R_DEAD);
    }
    return 0;
}

int ZombieObject::showAABB()
{
    SDL_FRect aabb = m_aabb;
    aabb.x = m_camera->getRenderX(aabb.x);
    aabb.y = m_camera->getRenderY(aabb.y);
    return SDL_RenderDrawRectF(m_loader->m_renderer, &aabb);
}

int ZombieObject::showShadow()
{
    SDL_FRect aabb = m_shadowRange;
    aabb.x = m_camera->getRenderX(aabb.x);
    aabb.y = m_camera->getRenderY(aabb.y);
    return SDL_RenderCopyF(m_loader->m_renderer, m_shadow, NULL, &aabb);
}

ZombieObject::~ZombieObject()
{}