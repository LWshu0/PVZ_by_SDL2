#include "Plants/PlantObject.h"

PlantObject::PlantObject(
    std::shared_ptr<AnimLoader> loader,
    std::shared_ptr<Camera> camera,
    const SDL_FPoint& init_point,
    int HP,
    uint64_t reload_ms,
    const SDL_FRect& aabb
) :
    AnimPlayer(loader, camera, init_point),
    GameObject(aabb),
    m_HP(HP),
    m_state(PlantState::IDLE),
    m_reloadMilliSecond(reload_ms)
{
    m_shadow = m_loader->m_imageRes->getTextureFrom("images/plantshadow.png");
}

PlantObject::PlantObject(
    std::shared_ptr<AnimLoader> loader,
    std::shared_ptr<Camera> camera,
    const SDL_FPoint& init_point,
    int HP,
    int reload_ms,
    const SDL_FPoint& root_point,
    float width,
    float height
) :
    AnimPlayer(loader, camera, init_point),
    GameObject(root_point, width, height),
    m_HP(HP),
    m_state(PlantState::IDLE),
    m_reloadMilliSecond(reload_ms)
{
    m_shadow = m_loader->m_imageRes->getTextureFrom("images/plantshadow.png");
}

int PlantObject::damage(int damege)
{
    m_HP -= damege;
    if (m_HP < 0)
    {
        changeDamageState(DamageState::R_Death);
    }
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