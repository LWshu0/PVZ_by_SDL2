#ifndef WSCIRCLE_H
#define WSCIRCLE_H

#include "wsShape.h"

/***************************************************
*                  圆形                            *
***************************************************/

class wsCircle :public wsShape {
protected:
    SDL_Point m_center;
    int m_radius;
public:
    wsCircle(const SDL_Point& center, int radius, SDL_Renderer* renderer);

    virtual int setPosition(int x, int y) override;
    
    /*将形状渲染到指定的位置*/
    virtual int renderShape() override;

    virtual bool isInShape(int x, int y) override;
    
    virtual ~wsCircle();
};

#endif