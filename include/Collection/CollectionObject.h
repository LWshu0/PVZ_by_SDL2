#ifndef COLLECTIONOBJECT_H
#define COLLECTIONOBJECT_H

#include "Core/AnimLoader.h"
#include "Core/AnimPlayer.h"
#include "Core/ObjectMotion.h"
#include "Core/Camera.h"
#include "Core/Timer.h"
#include "Core/GameObject.h"

enum CollectionType {
    Collection_Sun,
    CollectionMaxNum
};

/* 阳光, 银币, 金币, 钻石... */
class CollectionObject : public GameObject {
protected:
    std::shared_ptr<ObjectMotion> m_updater;
public:
    CollectionObject(const SDL_FRect& aabb) :GameObject(aabb) {};

    // 给定世界坐标, 复制一个物体
    virtual std::shared_ptr<CollectionObject> cloneCollection(float x, float y) = 0;

    virtual CollectionType getType() = 0;

    virtual int updateCollection(std::shared_ptr<Timer> timer)
    {
        return m_updater->step(this, timer->getDeltaTime());
    }

    inline void setMotion(std::shared_ptr<ObjectMotion> motion)
    {
        m_updater = motion;
    }

    virtual int renderCollection() = 0;

    virtual ~CollectionObject() {};
};

#endif