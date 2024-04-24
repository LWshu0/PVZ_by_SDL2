#ifndef EXTSDL_H
#define EXTSDL_H

#include "SDL.h"

/********************************************************
*                       整形运算                        *
*                                                       *
*********************************************************/

inline SDL_Point operator+(const SDL_Point& point1, const SDL_Point& point2) { return SDL_Point{ point1.x + point2.x,point1.y + point2.y }; }
inline SDL_Point operator+(const SDL_Point& point, int offset) { return SDL_Point{ point.x + offset,point.y + offset }; }
inline SDL_Point operator+(int offset, const SDL_Point& point) { return SDL_Point{ point.x + offset,point.y + offset }; }

inline SDL_Point operator-(const SDL_Point& point1, const SDL_Point& point2) { return SDL_Point{ point1.x - point2.x,point1.y - point2.y }; }
inline SDL_Point operator-(const SDL_Point& point, int offset) { return SDL_Point{ point.x - offset,point.y - offset }; }
inline SDL_Point operator-(int offset, const SDL_Point& point) { return SDL_Point{ offset - point.x,offset - point.y }; }
inline SDL_Point operator-(const SDL_Point& point) { return SDL_Point{ -point.x, -point.y }; }

inline SDL_Point operator*(const SDL_Point& point1, const SDL_Point& point2) { return SDL_Point{ point1.x * point2.x,point1.y * point2.y }; }
inline SDL_Point operator*(const SDL_Point& point, int factor) { return SDL_Point{ point.x * factor, point.y * factor }; }
inline SDL_Point operator*(int factor, const SDL_Point& point) { return SDL_Point{ point.x * factor, point.y * factor }; }

inline SDL_Point operator/(const SDL_Point& point1, const SDL_Point& point2) { return SDL_Point{ point1.x / point2.x,point1.y / point2.y }; }
inline SDL_Point operator/(const SDL_Point& point, int factor) { return SDL_Point{ point.x / factor, point.y / factor }; }
inline SDL_Point operator/(int factor, const SDL_Point& point) { return SDL_Point{ factor / point.x, factor / point.y }; }

inline int dot(const SDL_Point& point1, const SDL_Point& point2) { return point1.x * point2.x + point1.y * point2.y; }


/********************************************************
*                       浮点运算                        *
*                                                       *
*********************************************************/

inline SDL_FPoint operator+(const SDL_FPoint& point1, const SDL_FPoint& point2) { return SDL_FPoint{ point1.x + point2.x,point1.y + point2.y }; }
inline SDL_FPoint operator+(const SDL_FPoint& point, float offset) { return SDL_FPoint{ point.x + offset,point.y + offset }; }
inline SDL_FPoint operator+(float offset, const SDL_FPoint& point) { return SDL_FPoint{ point.x + offset,point.y + offset }; }

inline SDL_FPoint operator-(const SDL_FPoint& point1, const SDL_FPoint& point2) { return SDL_FPoint{ point1.x - point2.x,point1.y - point2.y }; }
inline SDL_FPoint operator-(const SDL_FPoint& point, float offset) { return SDL_FPoint{ point.x - offset,point.y - offset }; }
inline SDL_FPoint operator-(float offset, const SDL_FPoint& point) { return SDL_FPoint{ offset - point.x,offset - point.y }; }
inline SDL_FPoint operator-(const SDL_FPoint& point) { return SDL_FPoint{ -point.x, -point.y }; }

inline SDL_FPoint operator*(const SDL_FPoint& point1, const SDL_FPoint& point2) { return SDL_FPoint{ point1.x * point2.x,point1.y * point2.y }; }
inline SDL_FPoint operator*(const SDL_FPoint& point, float factor) { return SDL_FPoint{ point.x * factor, point.y * factor }; }
inline SDL_FPoint operator*(float factor, const SDL_FPoint& point) { return SDL_FPoint{ point.x * factor, point.y * factor }; }

inline SDL_FPoint operator/(const SDL_FPoint& point1, const SDL_FPoint& point2) { return SDL_FPoint{ point1.x / point2.x,point1.y / point2.y }; }
inline SDL_FPoint operator/(const SDL_FPoint& point, float factor) { return SDL_FPoint{ point.x / factor, point.y / factor }; }
inline SDL_FPoint operator/(float factor, const SDL_FPoint& point) { return SDL_FPoint{ factor / point.x, factor / point.y }; }

inline float dot(const SDL_FPoint& point1, const SDL_FPoint& point2) { return point1.x * point2.x + point1.y * point2.y; }


/********************************************************
*                   整形矩形运算                        *
*                                                       *
*********************************************************/

inline SDL_Rect operator+(const SDL_Rect& rect, const SDL_Point& point) { return SDL_Rect{ rect.x + point.x, rect.y + point.y, rect.w, rect.h }; }
inline SDL_Rect operator-(const SDL_Rect& rect, const SDL_Point& point) { return SDL_Rect{ rect.x - point.x, rect.y - point.y, rect.w, rect.h }; }


/********************************************************
*                   浮点矩形运算                        *
*                                                       *
*********************************************************/

inline SDL_FRect operator+(const SDL_FRect& rect, const SDL_FPoint& point) { return SDL_FRect{ rect.x + point.x, rect.y + point.y, rect.w, rect.h }; }
inline SDL_FRect operator-(const SDL_FRect& rect, const SDL_FPoint& point) { return SDL_FRect{ rect.x - point.x, rect.y - point.y, rect.w, rect.h }; }



#endif