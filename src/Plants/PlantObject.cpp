#include "Plants/PlantObject.h"

PlantObject::PlantObject(
    std::shared_ptr<AnimLoader> loader,
    std::shared_ptr<Camera> camera,
    const SDL_FPoint& init_point,
    int HP,
    const SDL_FRect& aabb
) :
    AnimPlayer(loader, camera, init_point),
    GameObject(aabb),
    m_HP(HP)
{}

PlantObject::PlantObject(
    std::shared_ptr<AnimLoader> loader,
    std::shared_ptr<Camera> camera,
    const SDL_FPoint& init_point,
    int HP,
    const SDL_FPoint& root_point,
    float width,
    float height
) :
    AnimPlayer(loader, camera, init_point),
    GameObject(root_point, width, height),
    m_HP(HP)
{
}

int PlantObject::damage(int damege)
{
    m_HP -= damege;
    if (m_HP < 0)
    {
        changeDamageState(DamageState::R_Death);
    }
}