#ifndef CAMERA_H
#define CAMERA_H

#include "SDL.h"

class Camera {
private:

    SDL_FPoint m_originPoint;

    SDL_FRect m_cameraViewport;

public:
    Camera();
    Camera(float x, float y, float w, float h);

    void initilize(float x, float y, float w, float h);

    void setOriginPoint(float x, float y);
    void setPosition(float x, float y);
    void move(float offset_x, float offset_y);

    inline float getRenderX(float x) { return x - m_cameraViewport.x; }
    inline float getRenderY(float y) { return y - m_cameraViewport.y; }
    inline SDL_FPoint getRenderPoint(const SDL_FPoint& point) { return SDL_FPoint{ point.x - m_cameraViewport.x, point.y - m_cameraViewport.y }; }
    inline float getClickX(int x) { return static_cast<float>(x) + m_cameraViewport.x; }
    inline float getClickY(int y) { return static_cast<float>(y) + m_cameraViewport.y; }
    inline SDL_FPoint getOriginPoint() { return m_originPoint; }
    inline SDL_FRect& getViewport() { return m_cameraViewport; }
    SDL_FRect clipFRect(const SDL_FRect& rect);

    inline float getWidth() { return m_cameraViewport.w; }
    inline float getHeight() { return m_cameraViewport.h; }
    inline float getX() { return m_cameraViewport.x; }
    inline float getY() { return m_cameraViewport.y; }
    inline float getLeft() { return m_cameraViewport.x; }
    inline float getRight() { return m_cameraViewport.x + m_cameraViewport.w; }
    inline float getTop() { return m_cameraViewport.y; }
    inline float getBottom() { return m_cameraViewport.y + m_cameraViewport.h; }

    ~Camera();
};

#endif