#ifndef CAMERA_H
#define CAMERA_H

class Camera
{
public:
    float m_width;
    float m_height;

    float m_x;
    float m_y;
    float m_bottom;
    float m_right;

    

public:
    Camera(float x, float y, float w, float h);

    int setPosition(float x, float y);

    ~Camera();
};

#endif