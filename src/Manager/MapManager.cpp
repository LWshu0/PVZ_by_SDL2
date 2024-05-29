#include "Manager/MapManager.h"
#include <fstream>
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "Map/GrassDayOneLine.h"

MapManager::MapManager() :m_map(nullptr)
{}

void MapManager::setMap(MapType type)
{
    switch (type)
    {
    case MapType::MapGrassDayOneLine:
        m_map = std::make_shared<GrassDayOneLine>();
        break;
    
    default:
        break;
    }
}

int MapManager::caculRow(float y)
{
    return static_cast<int>((y - m_map->getTopMargin()) / m_map->getCellHeight());
}

int MapManager::caculCol(float x)
{
    return static_cast<int>((x - m_map->getLeftMargin()) / m_map->getCellWidth());
}

bool MapManager::isValidCell(int row, int col)
{
    // 位置判断
    if (row < 0 || row >= m_map->getRow() || col < 0 || col >= m_map->getCol()) return false;
    // 地形判断
    if (m_map->getLandForm(row, col) != MapNode::LandForm::GRASS) return false;
    return true;
}

int MapManager::updateMap()
{
    return m_map->update();
}

int MapManager::renderMap()
{
    return m_map->render();
}

MapManager::~MapManager()
{}