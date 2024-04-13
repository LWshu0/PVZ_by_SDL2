#ifndef PEA_H
#define PEA_H

#include "BulletObject.h"

class Pea :public BulletObject {
protected:
    SDL_Texture* m_texture;
    int m_textureWidth;
    int m_textureHeight;
public:
    Pea(
        SDL_Renderer* renderer,
        SDL_Texture* texture,
        std::shared_ptr<Camera> camera,
        float x, float y);

    virtual std::shared_ptr<BulletObject> createBullet(float x, float y) override;
    virtual int renderBullet() override;
    virtual ~Pea() {};
};

#endif