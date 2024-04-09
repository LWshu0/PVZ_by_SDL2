#ifndef PEA_H
#define PEA_H

#include "BulletObject.h"

class Pea :public BulletObject {
public:
    Pea(
        SDL_Renderer* renderer,
        std::shared_ptr<Camera> camera,
        float x, float y);

    virtual std::shared_ptr<BulletObject> createBullet(float x, float y) override;
    virtual int renderBullet() override;
    virtual ~Pea() {};
};

#endif