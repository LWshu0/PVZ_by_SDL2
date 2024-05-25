#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "SDL.h"
#include <memory>

class GameObject {
public:
    SDL_FRect m_aabb;
public:
    GameObject(const SDL_FRect& aabb);
    /**
     * @brief Construct a new Game Object object
     * 
     * @param root_point 物体 aabb 碰撞箱的底边中点
     * @param width 碰撞箱的宽度
     * @param height 碰撞箱的高度
     */
    GameObject(const SDL_FPoint& root_point, float width, float height);

    virtual bool collision(std::shared_ptr<GameObject> other);
    virtual bool collision(const GameObject& other);

    bool inObject(int x, int y);
    bool inObject(float x, float y);
    inline SDL_FPoint getAABBPosition() { return SDL_FPoint{ m_aabb.x, m_aabb.y }; }
    inline SDL_FPoint getRootPosition() { return SDL_FPoint{ m_aabb.x + m_aabb.w / 2, m_aabb.y }; }

    // 渲染物体
    virtual int render() = 0;

    virtual ~GameObject();
};

#endif