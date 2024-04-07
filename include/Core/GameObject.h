#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "SDL.h"
#include <memory>

class GameObject {
public:
    SDL_FRect m_aabb;
    SDL_FPoint m_rootPoint;
public:
    GameObject(const SDL_FRect& aabb);
    GameObject(const SDL_FPoint& root_point, float width, float height);

    // virtual int update(uint64_t now_ms) = 0;

    // virtual int handleEvent(SDL_Event& event);

    virtual bool collision(std::shared_ptr<GameObject> other);
    virtual bool collision(const GameObject& other);

    inline SDL_FPoint getAABBPosition() { return SDL_FPoint{m_aabb.x, m_aabb.y}; }
    inline SDL_FPoint getRootPosition() { return m_rootPoint; }

    virtual ~GameObject();
};

#endif