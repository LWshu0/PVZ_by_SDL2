#ifndef SUN_H
#define SUN_H

#include "CollectionObject.h"

class Sun :public CollectionObject, public AnimPlayer {
protected:
    uint64_t m_movetimeMilliSecond;
    uint64_t m_lifetimeMilliSecond;
public:
    Sun(
        const SDL_FRect& aabb,
        std::shared_ptr<AnimLoader> loader,
        std::shared_ptr<Camera> camera
    );
    virtual std::shared_ptr<CollectionObject> cloneCollection(float x, float y) override;

    virtual CollectionType getType() override;
    virtual int updateCollection(std::shared_ptr<Timer> timer) override;
    virtual int renderCollection() override;
    ~Sun();
};



#endif