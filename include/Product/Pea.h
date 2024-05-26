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
        float x,
        float y
    );

    virtual std::shared_ptr<ProductObject> clone(float x, float y) override;
    virtual ProductType getType() override;
    virtual int render() override;
    virtual ~Pea() {};
};

#endif