#ifndef SUN_H
#define SUN_H

#include "CollectionObject.h"

class Sun :public CollectionObject {
protected:

    AnimPlayer m_animPlayer;

    uint64_t m_movetimeMilliSecond;
    uint64_t m_lifetimeMilliSecond;
public:
    Sun(
        const SDL_FRect& aabb,
        std::shared_ptr<AnimLoader> loader
    );
    virtual void initilize(float x, float y) override;
    virtual ProductType getType() override;
    virtual int update() override;
    virtual int render() override;
    ~Sun();
};

class SunFactory :public ObjectFactory<ProductObject> {
private:
    std::shared_ptr<AnimLoader> sun_loader;
public:
    SunFactory();
    virtual std::shared_ptr<ProductObject> create() override;
};

#endif