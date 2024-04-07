#include "Core/GameObject.h"

GameObject::GameObject(const SDL_FRect& aabb) :
    m_aabb(aabb),
    m_rootPoint({ aabb.x + aabb.w / 2, aabb.y + aabb.h })
{}

GameObject::GameObject(const SDL_FPoint& root_point, float width, float height) :
    m_aabb({ root_point.x - width / 2, root_point.y - height, width, height }),
    m_rootPoint(root_point)
{
    
}

// int GameObject::handleEvent(SDL_Event& event)
// {
//     return -1;
// }

bool GameObject::collision(std::shared_ptr<GameObject> other)
{
    if (this->m_aabb.x > other->m_aabb.x + other->m_aabb.w
        || other->m_aabb.x > this->m_aabb.x + this->m_aabb.w) return false;
    if (this->m_aabb.y > other->m_aabb.y + other->m_aabb.h
        || other->m_aabb.y > this->m_aabb.y + this->m_aabb.h) return false;
    return true;
}

bool GameObject::collision(const GameObject& other)
{
    if (this->m_aabb.x > other.m_aabb.x + other.m_aabb.w
        || other.m_aabb.x > this->m_aabb.x + this->m_aabb.w) return false;
    if (this->m_aabb.y > other.m_aabb.y + other.m_aabb.h
        || other.m_aabb.y > this->m_aabb.y + this->m_aabb.h) return false;
    return true;
}

GameObject::~GameObject()
{}