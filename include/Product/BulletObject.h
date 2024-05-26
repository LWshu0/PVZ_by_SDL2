#ifndef BULLETOBJECT_H
#define BULLETOBJECT_H

#include "ProductObject.h"

class BulletObject :public ProductObject {
protected:
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
        const SDL_FPoint& offsetShadow,
        float shadowWidth,
        float shadowHeight,
        int dam
    ) :
        ProductObject(aabb),
        m_offsetShadow(offsetShadow),
        m_shadowWidth(shadowWidth),
        m_shadowHeight(shadowHeight),
        m_damage(dam)
    {
        m_shadow = GlobalVars::getInstance().textureRes.getTextureFrom("images/plantshadow.png");
    }

    // 返回子弹的伤害
    inline int getDamage() { return m_damage; }

    virtual ~BulletObject() {};
};

#endif