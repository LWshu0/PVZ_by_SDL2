#ifndef WSRECTANGLE_H
#define WSRECTANGLE_H

#include "wsShape.h"

/***************************************************
*                  矩形                            *
***************************************************/

class wsRectangle :public wsShape {
public:
    SDL_Rect m_range;
public:
    wsRectangle(const SDL_Rect& range);

    virtual int setPosition(int x, int y) override;

    /*将形状渲染到指定的位置*/
    virtual int renderShape() override;

    virtual bool isInShape(int x, int y) override;

    virtual ~wsRectangle();
};

#endif