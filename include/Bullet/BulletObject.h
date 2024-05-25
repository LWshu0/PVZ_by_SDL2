#ifndef BULLETOBJECT_H
#define BULLETOBJECT_H
#include "Core/GameObject.h"
#include "Core/ObjectMotion.h"
#include "Core/GlobalVars.h"
#include <memory>

enum BulletType {
    BulletPea,
    MaxBulletType   // 新的类型添加到该类型上方 此变量可指定子弹的种类数量
};

class BulletObject :public GameObject {
protected:
    std::shared_ptr<ObjectMotion> m_updater;    // 子弹更新器(用于控制子弹的运动轨迹)

    // 子弹属性
    int m_damage;

    // 子弹影子
    SDL_Texture* m_shadow;
    SDL_FPoint m_offsetShadow;
    float m_shadowWidth;
    float m_shadowHeight;
public:
    BulletObject(
        const SDL_FRect& aabb,
        SDL_Texture* shadow,
        const SDL_FPoint& offsetShadow,
        float shadowWidth,
        float shadowHeight,
        int dam
    ) :
        GameObject(aabb),
        m_shadow(shadow),
        m_offsetShadow(offsetShadow),
        m_shadowWidth(shadowWidth),
        m_shadowHeight(shadowHeight),
        m_damage(dam)
    {}

    // 根据模板创建一个新的子弹
    // 与模板仅位置不同
    virtual std::shared_ptr<BulletObject> cloneBullet(float x, float y) = 0;

    // 更新子弹的位置等状态
    virtual int updateBullet()
    {
        return m_updater->step(this, GlobalVars::getInstance().timer.getDeltaTime());
    }

    // 设置子弹的运动更新器
    inline void setMotion(std::shared_ptr<ObjectMotion> motion)
    {
        m_updater = motion;
    }
    // 返回子弹的伤害
    inline int getDamage() { return m_damage; }

    virtual ~BulletObject() {};
};

#endif