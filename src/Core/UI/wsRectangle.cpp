#include "Core/UI/wsRectangle.h"

wsRectangle::wsRectangle(const SDL_Rect& range) :
    wsShape(),
    m_range(range)
{}

int wsRectangle::setPosition(int x, int y)
{
    m_range.x = x;
    m_range.y = y;
    return 0;
}

int wsRectangle::renderShape()
{
    if (!m_isValid) return -1;
    SDL_SetRenderDrawColor(GlobalVars::getInstance().renderer, 255, 255, 255, 255);
    return SDL_RenderDrawRect(GlobalVars::getInstance().renderer, &m_range);
}

bool wsRectangle::isInShape(int x, int y)
{
    SDL_Point point{ x, y };
    return SDL_PointInRect(&point, &m_range);
}

wsRectangle::~wsRectangle() {}