#include "Manager/MapManager.h"

MapManager::MapManager(
    SDL_Renderer* renderer,
    std::shared_ptr<TextureRes> res,
    std::shared_ptr<Camera> camera
) :
    m_renderer(renderer),
    m_textureRes(res),
    m_camera(camera),
    m_mapWidthPixel(0),
    m_mapHeightPixel(0),
    m_leftMargin(0),
    m_rightMargin(0),
    m_topMargin(0),
    m_bottomMargin(0),
    m_cellHeight(0),
    m_cellWidth(0)
{
    // 初始化模板数组
    m_mapTemplate.resize(MapType::MaxMapType);
    // 初始化模板
    m_mapTemplate[MapType::MapGrassDayOneLine] = std::make_shared<GrassDayOneLine>(m_textureRes);

}

int MapManager::setMap(
    float width,
    float height,
    MapType type
)
{
    m_mapType = type;
    m_mapTemplate[m_mapType]->initilizeMapManager(this, width, height);
    m_mapTemplate[m_mapType]->resetMap(m_mapRunTime);
    return 0;
}

int MapManager::caculRow(float y)
{
    return static_cast<int>((y - m_topMargin) / m_cellHeight);
}

int MapManager::caculCol(float x)
{
    return static_cast<int>((x - m_leftMargin) / m_cellWidth);
}

int MapManager::renderMap()
{
    SDL_FRect map_rect{
        m_camera->getRenderX(0.0f),
        m_camera->getRenderY(0.0f),
        m_mapWidthPixel,
        m_mapHeightPixel
    };
    return SDL_RenderCopyF(m_renderer, m_bkTexture, NULL, &map_rect);
}

MapManager::~MapManager()
{}