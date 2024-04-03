#include "Core/Camera.h"

Camera::Camera(float x, float y, float w, float h) :
    m_x(x),
    m_y(y),
    m_width(w),
    m_height(h),
    m_bottom(y + h),
    m_right(x + w)
{}

int Camera::setPosition(float x, float y)
{
    m_x = x;
    m_y = y;
    m_bottom = m_y + m_height;
    m_right = m_x + m_width;
}

Camera::~Camera()
{}