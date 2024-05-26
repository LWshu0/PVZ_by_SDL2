#ifndef COLLECTIONOBJECT_H
#define COLLECTIONOBJECT_H

#include "ProductObject.h"

/* 阳光, 银币, 金币, 钻石... */
class CollectionObject : public ProductObject {
public:
    CollectionObject(const SDL_FRect& aabb) :ProductObject(aabb) {};

    virtual ~CollectionObject() {};
};

#endif