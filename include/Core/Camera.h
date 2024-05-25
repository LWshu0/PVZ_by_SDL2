#ifndef CAMERA_H
#define CAMERA_H

#include "SDL.h"

class Camera {
private:
    float m_width;
    float m_height;

    float m_x;
    float m_y;
    float m_bottom;
    float m_right;

public:
    Camera();
    Camera(float x, float y, float w, float h);

    void initilize(float x, float y, float w, float h);
    
    int setPosition(float x, float y);
    int move(float offset_x, float offset_y);

    inline float getRenderX(float x) { return x - m_x; }
    inline float getRenderY(float y) { return y - m_y; }
    inline SDL_FPoint getRenderPoint(const SDL_FPoint& point) { return SDL_FPoint{ point.x - m_x, point.y - m_y }; }
    inline float getClickX(int x) { return static_cast<float>(x) + m_x; }
    inline float getClickY(int y) { return static_cast<float>(y) + m_y; }

    inline float getWidth() { return m_width; }
    inline float getHeight() { return m_height; }
    inline float getX() { return m_x; }
    inline float getY() { return m_y; }
    inline float getLeft() { return m_x; }
    inline float getRight() { return m_right; }
    inline float getTop() { return m_y; }
    inline float getBottom() { return m_bottom; }

    ~Camera();
};

#endif