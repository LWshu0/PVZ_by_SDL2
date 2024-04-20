#include "Core/UI/wsTriangles.h"

wsTriangles::wsTriangles(
    std::vector<SDL_FPoint>& points,
    std::vector<int>& idxes,
    SDL_Renderer* renderer
) :
    wsShape(renderer)
{

    for (auto& point : points)
    {
        m_points.push_back(point);
    }
    m_points.shrink_to_fit();
    
    for (auto& i : idxes)
    {
        if (i < points.size()) m_idxes.push_back(i);
    }
    while (m_idxes.size() % 3 != 0) m_idxes.pop_back();
    m_idxes.shrink_to_fit();
    if (m_idxes.size() != 0) m_isValid = true;
    else m_isValid = false;
}

int wsTriangles::setPosition(int x, int y)
{
    if (!m_isValid) return -1;
    int offset_x = x - m_points[0].x, offset_y = y - m_points[0].y;
    for (auto& point : m_points)
    {
        point.x += offset_x;
        point.y += offset_y;
    }
    return 0;
}

int wsTriangles::renderShape()
{
    if (!m_isValid) return -1;
    for (int ii = 0;ii < m_idxes.size();ii += 3)
    {
        SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
        SDL_RenderDrawLineF(m_renderer, m_points[m_idxes[ii]].x, m_points[m_idxes[ii]].y, m_points[m_idxes[ii + 1]].x, m_points[m_idxes[ii + 1]].y);
        SDL_RenderDrawLineF(m_renderer, m_points[m_idxes[ii]].x, m_points[m_idxes[ii]].y, m_points[m_idxes[ii + 2]].x, m_points[m_idxes[ii + 2]].y);
        SDL_RenderDrawLineF(m_renderer, m_points[m_idxes[ii + 2]].x, m_points[m_idxes[ii + 2]].y, m_points[m_idxes[ii + 1]].x, m_points[m_idxes[ii + 1]].y);
    }
    return 0;
}

bool wsTriangles::isInShape(int x, int y)
{
    if (!m_isValid) return false;
    
    for (int i = 0;i < m_idxes.size();i += 3)
        {
            bool p1 = (m_points[m_idxes[i]].x - x) * (m_points[m_idxes[i + 1]].y - m_points[m_idxes[i]].y) - (m_points[m_idxes[i + 1]].x - m_points[m_idxes[i]].x) * (m_points[m_idxes[i]].y - y) > 0;
            bool p2 = (m_points[m_idxes[i + 1]].x - x) * (m_points[m_idxes[i + 2]].y - m_points[m_idxes[i + 1]].y) - (m_points[m_idxes[i + 2]].x - m_points[m_idxes[i + 1]].x) * (m_points[m_idxes[i + 1]].y - y) > 0;
            bool p3 = (m_points[m_idxes[i + 2]].x - x) * (m_points[m_idxes[i]].y - m_points[m_idxes[i + 2]].y) - (m_points[m_idxes[i]].x - m_points[m_idxes[i + 2]].x) * (m_points[m_idxes[i + 2]].y - y) > 0;
            if (p1 == p2 && p2 == p3) return true;
        }
    
    return false;
}


wsTriangles::~wsTriangles() {}