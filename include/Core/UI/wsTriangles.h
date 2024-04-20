#ifndef WSTRIANGLE_H
#define WSTRIANGLE_H

#include "wsShape.h"
#include <vector>

/***************************************************
*                  三角形                          *
*           第一个顶点代表位置                      *
***************************************************/

class wsTriangles :public wsShape {
protected:
    std::vector<SDL_FPoint> m_points;
    std::vector<int> m_idxes;
public:
    wsTriangles(
        std::vector<SDL_FPoint>& points,
        std::vector<int>& idxes,
        SDL_Renderer* renderer
    );

    virtual int setPosition(int x, int y) override;

    /*将形状渲染到指定的位置*/
    virtual int renderShape() override;

    virtual bool isInShape(int x, int y) override;

    virtual ~wsTriangles();
};

#endif