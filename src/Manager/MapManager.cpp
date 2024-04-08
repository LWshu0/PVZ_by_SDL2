#include "Manager/MapManager.h"

MapManager::MapManager() :
    m_mapWidthPixel(0),
    m_mapHeightPixel(0),
    m_leftMargin(0),
    m_rightMargin(0),
    m_topMargin(0),
    m_bottomMargin(0),
    m_cellHeight(0),
    m_cellWidth(0),
    m_initilizer(nullptr)
{}

int MapManager::setMap(
    float width,
    float height,
    float left,
    float right,
    float top,
    float bottom,
    std::shared_ptr<MapInitilizer> mapTemplate
)
{
    m_mapWidthPixel = width;
    m_mapHeightPixel = height;
    m_leftMargin = left;
    m_rightMargin = right;
    m_topMargin = top;
    m_bottomMargin = bottom;
    m_cellWidth = (right - left) / mapTemplate->getCol();
    m_cellHeight = (bottom - top) / mapTemplate->getRow();
    m_rowNum = mapTemplate->getRow();
    m_colNum = mapTemplate->getCol();
    mapTemplate->initilizeMapTemplate();
    m_initilizer = mapTemplate;
    m_initilizer->resetMap(m_mapRunTime);
    return 0;
}

MapManager::~MapManager()
{}