#ifndef WSSHAPE_H
#define WSSHAPE_H

#include "SDL.h"

class wsShape {
protected:
    bool m_isValid;
    SDL_Renderer* m_renderer;
public:
    wsShape(SDL_Renderer* renderer);

    /*将形状渲染到指定的位置*/
    virtual int renderShape() = 0;

    virtual int setPosition(int x, int y) = 0;
    // 判断坐标是否在形状中
    virtual bool isInShape(int x, int y) = 0;
    
    virtual ~wsShape();
};

/***************************************************
*                  常用方法                         *
***************************************************/

int SDL_RenderDrawCircle(SDL_Renderer* renderer, int x, int y, int radius);

int SDL_RenderFillCircle(SDL_Renderer* renderer, int x, int y, int radius);

// 绘制一个圆形边框
// 宽度为一个像素, 不在表面上的像素点不会绘制
int SDL_DrawCircle(SDL_Surface* surface, int x, int y, int radius, Uint32 color);

// 绘制填充圆型到表面
// 进行边界检查, 超出边界的不绘制
int SDL_FillCircle(SDL_Surface* surface, int x, int y, int radius, Uint32 color);

// 将一个 RGBA 格式的表面的 x, y 处的像素指针返回
// 需要首先锁定表面
Uint32* SDL_SurfacePix(SDL_Surface* _surface, int x, int y);
// 向表面中写入一个像素, 0 代表成功, 否则失败
// 仅在锁定表面后使用
int SDL_SurfaceWritePix(SDL_Surface* _surface, int x, int y, Uint32 color);

// 将 mask 中非 0 位置的比特保留在 surface 中
void SDL_SurfaceMask(SDL_Surface* surface, SDL_Surface* mask);

#endif