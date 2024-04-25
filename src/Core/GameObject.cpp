#include "Core/GameObject.h"

GameObject::GameObject(const SDL_FRect& aabb) :
    m_aabb(aabb)
{}

GameObject::GameObject(const SDL_FPoint& root_point, float width, float height) :
    m_aabb({ root_point.x - width / 2, root_point.y - height, width, height })
{}

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
bool GameObject::inObject(int x, int y)
{
    if (this->m_aabb.x > x
        || x > this->m_aabb.x + this->m_aabb.w) return false;
    if (this->m_aabb.y > y
        || y > this->m_aabb.y + this->m_aabb.h) return false;
    return true;
}

bool GameObject::inObject(float x, float y)
{
    if (this->m_aabb.x > x
        || x > this->m_aabb.x + this->m_aabb.w) return false;
    if (this->m_aabb.y > y
        || y > this->m_aabb.y + this->m_aabb.h) return false;
    return true;
}

GameObject::~GameObject()
{}