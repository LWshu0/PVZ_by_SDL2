#ifndef CAMERA_H
#define CAMERA_H

#include "SDL.h"

class Camera {
private:
    SDL_FRect m_cameraViewport;

public:
    Camera();
    Camera(float x, float y, float w, float h);

    void initilize(float x, float y, float w, float h);

    void setPosition(float x, float y);
    void move(float offset_x, float offset_y);

    inline float getRenderX(float x) { return x - m_cameraViewport.x; }
    inline float getRenderY(float y) { return y - m_cameraViewport.y; }
    inline int getRenderX(int x) { return x - static_cast<int>(m_cameraViewport.x); }
    inline int getRenderY(int y) { return y - static_cast<int>(m_cameraViewport.y); }
    inline SDL_FPoint getRenderPoint(const SDL_FPoint& point) { return SDL_FPoint{ point.x - m_cameraViewport.x, point.y - m_cameraViewport.y }; }
    inline float getClickX(int x) { return static_cast<float>(x) + m_cameraViewport.x; }
    inline float getClickY(int y) { return static_cast<float>(y) + m_cameraViewport.y; }
    inline SDL_FRect& getViewport() { return m_cameraViewport; }

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