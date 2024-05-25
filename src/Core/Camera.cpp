#include "Core/Camera.h"

Camera::Camera() :
    m_x(0), m_y(0), m_width(0), m_height(0), m_bottom(0), m_right(0)
{}

Camera::Camera(float x, float y, float w, float h)
{
    initilize(x, y, w, h);
}

void Camera::initilize(float x, float y, float w, float h)
{
    m_x = x;
    m_y = y;
    m_width = w;
    m_height = h;
    m_bottom = y + h;
    m_right = x + w;
}

int Camera::setPosition(float x, float y)
{
    m_x = x;
    m_y = y;
    m_bottom = m_y + m_height;
    m_right = m_x + m_width;
    return 0;
}

int Camera::move(float offset_x, float offset_y)
{
    m_x += offset_x;
    m_y += offset_y;
    m_bottom += offset_y;
    m_right += offset_x;
    return 0;
}

Camera::~Camera()
{}