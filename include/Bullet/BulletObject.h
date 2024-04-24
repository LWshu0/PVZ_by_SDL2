#ifndef BULLETOBJECT_H
#define BULLETOBJECT_H
#include "Core/Camera.h"
#include "Core/GameObject.h"
#include "Core/ObjectMotion.h"
#include <memory>

enum BulletType {
    BulletPea,
    MaxBulletType   // 新的类型添加到该类型上方 此变量可指定子弹的种类数量
};

class BulletObject :public GameObject {
protected:
    SDL_Renderer* m_renderer;                   // 渲染器
    std::shared_ptr<Camera> m_camera;           // 相机
    std::shared_ptr<ObjectMotion> m_updater;    // 子弹更新器(用于控制子弹的运动轨迹)

    // 子弹属性
    int m_damage;
public:
    BulletObject(
        SDL_Renderer* renderer,
        std::shared_ptr<Camera> camera,
        const SDL_FRect& aabb,
        int dam
    ) :
        GameObject(aabb),
        m_renderer(renderer),
        m_camera(camera),
        m_damage(dam)
    {}

    // 根据模板创建一个新的子弹
    // 与模板仅位置不同
    virtual std::shared_ptr<BulletObject> cloneBullet(float x, float y) = 0;

    // 更新子弹的位置等状态
    virtual int updateBullet(uint64_t delta_ms)
    {
        return m_updater->step(this, delta_ms);
    }

    // 设置子弹的运动更新器
    inline void setMotion(std::shared_ptr<ObjectMotion> motion)
    {
        m_updater = motion;
    }
    // 返回子弹的伤害
    inline int getDamage() { return m_damage; }

    // 渲染子弹(可能有动画)
    virtual int renderBullet() = 0;

    virtual ~BulletObject() {};
};

#endif