#include "Core/Camera.h"

Camera::Camera() :
    m_cameraViewport{ 0.0f,0.0f,0.0f,0.0f }
{}

Camera::Camera(float x, float y, float w, float h)
{
    initilize(x, y, w, h);
}

void Camera::initilize(float x, float y, float w, float h)
{
    m_cameraViewport = SDL_FRect{ x, y, w, h };
}

void Camera::setOriginPoint(float x, float y)
{
    m_originPoint = SDL_FPoint{ x, y };
}

void Camera::setPosition(float x, float y)
{
    m_cameraViewport.x = x;
    m_cameraViewport.y = y;
}

void Camera::move(float offset_x, float offset_y)
{
    m_cameraViewport.x += offset_x;
    m_cameraViewport.y += offset_y;
}

Camera::~Camera()
{}