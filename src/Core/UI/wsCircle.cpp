#include "Core/UI/wsCircle.h"
#include "Core/CoreVars.h"

wsCircle::wsCircle(const SDL_Point& center, int radius) :
    wsShape(),
    m_center(center),
    m_radius(radius)
{}


int wsCircle::setPosition(int x, int y)
{
    m_center.x = x;
    m_center.y = y;
    return 0;
}

int wsCircle::renderShape()
{
    if (!m_isValid) return -1;

    SDL_SetRenderDrawColor(CoreVars::getInstance().renderer, 255, 255, 255, 255);
    
    // SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    return SDL_RenderDrawCircle(CoreVars::getInstance().renderer, m_center.x, m_center.y, m_radius);
    
    return -1;
}

bool wsCircle::isInShape(int x, int y)
{
    int offsetx = x - m_center.x;
    int offsety = y - m_center.y;
    return offsetx * offsetx + offsety * offsety <= m_radius * m_radius;
}

wsCircle::~wsCircle() {}