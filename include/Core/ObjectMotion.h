#ifndef OBJECTMOTION_H
#define OBJECTMOTION_H

#include "GameObject.h"

class ObjectMotion {
public:
    ObjectMotion() {};
    virtual int step(GameObject* object, uint64_t delta_ms) = 0;
    virtual ~ObjectMotion() {};
};

class MotionSpeed :public ObjectMotion {
protected:
    float m_speedX, m_speedY;
public:
    MotionSpeed(float sx, float sy) :
        ObjectMotion(),
        m_speedX(sx),
        m_speedY(sy)
    {}
    virtual int step(GameObject* object, uint64_t delta_ms) override;
    virtual ~MotionSpeed() {}
};

#endif