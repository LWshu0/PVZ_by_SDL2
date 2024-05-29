#ifndef PRODUCTOBJECT_H
#define PRODUCTOBJECT_H
#include "Core/AnimLoader.h"
#include "Core/AnimPlayer.h"
#include "Core/ObjectMotion.h"
#include "Core/GameObject.h"
#include "Core/GlobalVars.h"

enum ProductState {
    Product_MOVE,         // 运动状态
    Product_BREAK,        // 破碎动画
    Product_DELETE        // 删除状态, 其对象可以被销毁
};

enum ProductType {
    // Bullet
    PeaType,
    // Collection
    SunType,
    MaxProductNum
};

class ProductObject : public GameObject {
protected:
    ProductState m_state;
    std::shared_ptr<ObjectMotion> m_updater;
public:
    ProductObject(const SDL_FRect& aabb);

    virtual std::shared_ptr<ProductObject> clone(float x, float y) = 0;
    inline ProductState getState() { return m_state; }
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