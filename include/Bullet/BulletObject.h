#ifndef BULLETOBJECT_H
#define BULLETOBJECT_H
#include "Core/Camera.h"
#include "Core/GameObject.h"
#include "Core/ObjectMotion.h"
#include <memory>

class BulletObject :public GameObject {
protected:
    int m_damage;
    SDL_Renderer* m_renderer;
    std::shared_ptr<Camera> m_camera;
    std::shared_ptr<ObjectMotion> m_updater;
public:
    BulletObject(
        SDL_Renderer* renderer,
        std::shared_ptr<Camera> camera,
        const SDL_FRect& aabb
    ) :
        GameObject(aabb),
        m_renderer(renderer),
        m_camera(camera)
    {}

    virtual std::shared_ptr<BulletObject> createBullet(float x, float y) = 0;

    virtual int updateBullet(uint64_t delta_ms)
    {
        return m_updater->step(this, delta_ms);
    } 

    inline void setMotion(std::shared_ptr<ObjectMotion> motion)
    {
        m_updater = motion;
    }
    inline int getDamage() { return m_damage; }

    virtual int renderBullet() = 0;

    virtual ~BulletObject() {};
};

#endif