#include "Map/GrassDayOneLine.h"
#include "Core/GlobalVars.h"
GrassDayOneLine::GrassDayOneLine() :
    MapObject()
{
    // m_bkTexture = res->getTextureFrom("images/background1unsodded.jpg");
    load("map/1-1.json");
    m_oneLineGrassTexture = GlobalVars::getInstance().textureRes.getTextureWithMask("images/sod1row.jpg", "images/sod1row_.png");
    SDL_QueryTexture(m_oneLineGrassTexture, NULL, NULL, &m_grassWidth, &m_grassHeight);
}

int GrassDayOneLine::update()
{
    return 0;
}

int GrassDayOneLine::render()
{
    SDL_FRect map_rect{
        GlobalVars::getInstance().camera.getRenderX(0.0f),
        GlobalVars::getInstance().camera.getRenderY(0.0f),
        m_mapWidthPixel,
        m_mapHeightPixel
    };
    SDL_RenderCopyF(GlobalVars::getInstance().renderer, m_bkTexture, NULL, &map_rect);
    SDL_Rect grass_rect{
        (int)GlobalVars::getInstance().camera.getRenderX(237.0f),
        (int)GlobalVars::getInstance().camera.getRenderY(270.0f),
        m_grassWidth,
        m_grassHeight
    };
    SDL_RenderCopy(GlobalVars::getInstance().renderer, m_oneLineGrassTexture, NULL, &grass_rect);
    return 0;
}

GrassDayOneLine::~GrassDayOneLine()
{}