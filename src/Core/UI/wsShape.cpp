#include "Core/UI/wsShape.h"

wsShape::wsShape(SDL_Renderer* renderer) :
    m_renderer(renderer)
{
    m_isValid = (m_renderer != nullptr);
}

wsShape::~wsShape()
{}

/***************************************************
*                  常用方法                         *
***************************************************/

int SDL_RenderDrawCircle(SDL_Renderer* renderer, int x, int y, int radius)
{
    int offsetx = radius;
    int offsety = 0;
    int d = 1 - radius;

    while (offsety <= offsetx)
    {
        SDL_RenderDrawPoint(renderer, x + offsetx, y + offsety);
        SDL_RenderDrawPoint(renderer, x + offsety, y + offsetx);
        SDL_RenderDrawPoint(renderer, x - offsetx, y + offsety);
        SDL_RenderDrawPoint(renderer, x - offsety, y + offsetx);
        SDL_RenderDrawPoint(renderer, x + offsetx, y - offsety);
        SDL_RenderDrawPoint(renderer, x + offsety, y - offsetx);
        SDL_RenderDrawPoint(renderer, x - offsetx, y - offsety);
        SDL_RenderDrawPoint(renderer, x - offsety, y - offsetx);

        if (d <= 0)
        {
            d = d + 2 * offsety + 3;
        }
        else
        {
            d = d - 2 * offsetx + 2 * offsety + 5;
            offsetx -= 1;
        }
        offsety += 1;
    }

    return 0;
}


int SDL_RenderFillCircle(SDL_Renderer* renderer, int x, int y, int radius)
{
    if (0 >= radius) return -1;     // 不需要绘制
    int offsetx = radius;
    int offsety = 1;
    int d = 4 - radius;
    // 避免第一次的重复画线
    SDL_RenderDrawLine(renderer, x - radius, y, x + radius, y);
    // 循环画横线
    while (offsety <= offsetx)
    {
        SDL_RenderDrawLine(renderer, x - offsetx, y + offsety, x + offsetx, y + offsety);
        SDL_RenderDrawLine(renderer, x - offsetx, y - offsety, x + offsetx, y - offsety);
        if (d <= 0)
        {
            d = d + 2 * offsety + 3;
        }
        else
        {
            SDL_RenderDrawLine(renderer, x - offsety, y + offsetx, x + offsety, y + offsetx);
            SDL_RenderDrawLine(renderer, x - offsety, y - offsetx, x + offsety, y - offsetx);
            d = d - 2 * offsetx + 2 * offsety + 5;
            offsetx -= 1;
        }
        offsety += 1;
    }
    return 0;
}

int SDL_DrawCircle(SDL_Surface* surface, int x, int y, int radius, Uint32 color)
{
    int offsetx = radius;
    int offsety = 0;
    int d = 1 - radius;

    if (SDL_MUSTLOCK(surface))
    {    // 锁定表面以获取访问权限
        SDL_LockSurface(surface);
    }

    while (offsety <= offsetx)
    {
        SDL_SurfaceWritePix(surface, x + offsetx, y + offsety, color);
        SDL_SurfaceWritePix(surface, x + offsety, y + offsetx, color);
        SDL_SurfaceWritePix(surface, x - offsetx, y + offsety, color);
        SDL_SurfaceWritePix(surface, x - offsety, y + offsetx, color);
        SDL_SurfaceWritePix(surface, x + offsetx, y - offsety, color);
        SDL_SurfaceWritePix(surface, x + offsety, y - offsetx, color);
        SDL_SurfaceWritePix(surface, x - offsetx, y - offsety, color);
        SDL_SurfaceWritePix(surface, x - offsety, y - offsetx, color);

        if (d <= 0)
        {
            d = d + 2 * offsety + 3;
        }
        else
        {
            d = d - 2 * offsetx + 2 * offsety + 5;
            offsetx -= 1;
        }
        offsety += 1;
    }

    // 解锁表面
    if (SDL_MUSTLOCK(surface))
    {
        SDL_UnlockSurface(surface);
    }

    return 0;
}

int SurfaceDrawRow(SDL_Surface* surface, int x, int y, int length, Uint32 color)
{
    if (y < 0 || y >= surface->h) return -1;
    int start = x < 0 ? 0 : x;
    int end = x + length >= surface->w ? surface->w - 1 : x + length - 1;
    length = end - start + 1;
    Uint32* start_ptr = SDL_SurfacePix(surface, start, y);
    if (nullptr != start_ptr)
    {
        for (int i = 0;i < length;i++)
        {
            start_ptr[i] = color;
        }
        return 0;
    }
    else return -1;
}

int SDL_FillCircle(SDL_Surface* surface, int x, int y, int radius, Uint32 color)
{
    if (0 >= radius || x < -radius || x >= surface->w + radius || y < -radius || y >= surface->h + radius) return -1;     // 不需要绘制
    int offsetx = radius;
    int offsety = 1;
    int d = 4 - radius;
    if (SDL_MUSTLOCK(surface))  // 锁定表面以获取访问权限
    {
        SDL_LockSurface(surface);
    }
    // 避免第一次的重复画线
    SurfaceDrawRow(surface, x - radius, y, 2 * radius, color);
    // 循环画线
    while (offsety <= offsetx)
    {
        SurfaceDrawRow(surface, x - offsetx, y - offsety, offsetx * 2, color);
        SurfaceDrawRow(surface, x - offsetx, y + offsety, offsetx * 2, color);
        if (d <= 0)
        {
            d = d + 2 * offsety + 3;
        }
        else
        {
            SurfaceDrawRow(surface, x - offsety, y - offsetx, offsety * 2, color);
            SurfaceDrawRow(surface, x - offsety, y + offsetx, offsety * 2, color);
            d = d - 2 * offsetx + 2 * offsety + 5;
            offsetx -= 1;
        }
        offsety += 1;
    }

    if (SDL_MUSTLOCK(surface))  // 解锁表面
    {
        SDL_UnlockSurface(surface);
    }
    return 0;
}

Uint32* SDL_SurfacePix(SDL_Surface* _surface, int x, int y)
{
    // 判断坐标是否在 surface 范围内
    if (x < 0 || y < 0 || x >= _surface->w || y >= _surface->h) return nullptr;
    size_t pix_size = sizeof(Uint32);
    int idx_offset = y * _surface->pitch + x * pix_size;
    return (Uint32*)((Uint8*)_surface->pixels + idx_offset);
}

int SDL_SurfaceWritePix(SDL_Surface* _surface, int x, int y, Uint32 color)
{
    Uint32* pix_ptr = SDL_SurfacePix(_surface, x, y);
    if (nullptr != pix_ptr)
    {
        *pix_ptr = color;
        return 0;
    }
    return -1;
}

void SDL_SurfaceMask(SDL_Surface* surface, SDL_Surface* mask)
{
    // 锁定表面以获取访问权限
    if (SDL_MUSTLOCK(surface)) SDL_LockSurface(surface);
    if (SDL_MUSTLOCK(mask)) SDL_LockSurface(mask);
    // 实际数据
    Uint8* pix1 = (Uint8*)surface->pixels;
    Uint8* pix2 = (Uint8*)mask->pixels;
    // 逐位掩码
    size_t length = surface->pitch * surface->h;
    for (size_t i = 0;i < length;i++)
    {
        if (pix2[i] == 0) pix1[i] = 0;
    }
    // 解锁表面
    if (SDL_MUSTLOCK(surface)) SDL_UnlockSurface(surface);
    if (SDL_MUSTLOCK(mask)) SDL_UnlockSurface(mask);
}