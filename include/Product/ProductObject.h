#ifndef PRODUCTOBJECT_H
#define PRODUCTOBJECT_H
#include "Core/AnimLoader.h"
#include "Core/AnimPlayer.h"
#include "Core/ObjectMotion.h"
#include "Core/GameObject.h"
#include "Core/GlobalVars.h"

enum ProductType {
    // Bullet
    PeaType,
    // Collection
    SunType,
    MaxProductNum
};

class ProductObject : public GameObject {
protected:
    std::shared_ptr<ObjectMotion> m_updater;
public:
    ProductObject(const SDL_FRect& aabb);

    virtual std::shared_ptr<ProductObject> clone(float x, float y) = 0;
    virtual ProductType getType() = 0;
    virtual int update() override
    {
        return m_updater->step(this, GlobalVars::getInstance().timer.getDeltaTime());
    }

    // 设置运动更新器
    inline void setMotion(std::shared_ptr<ObjectMotion> motion)
    {
        m_updater = motion;
    }

    ~ProductObject();
};

#endif