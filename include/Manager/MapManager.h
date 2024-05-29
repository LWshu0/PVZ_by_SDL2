#ifndef MAPMANAGER_H
#define MAPMANAGER_H

#include <vector>
#include <memory>
#include "Core/GlobalVars.h"
#include "Map/MapObject.h"

enum MapType {
    MapGrassDayOneLine,
    // 此处添加新的地图 ...
    MaxMapType
};

class MapManager {
private:

    std::shared_ptr<MapObject> m_map;

public:
    MapManager();

    void setMap(MapType type);

    inline float getWidth() { return m_map->getWidth(); }
    inline float getHeight() { return m_map->getHeight(); }
    inline float getLeftMargin() { return m_map->getLeftMargin(); }
    inline float getRightMargin() { return m_map->getRightMargin(); }
    inline float getTopMargin() { return m_map->getTopMargin(); }
    inline float getBottomMargin() { return m_map->getBottomMargin(); }
    inline float getCellWidth() { return m_map->getCellWidth(); }
    inline float getCellHeight() { return m_map->getCellHeight(); }
    inline int getRow() { return m_map->getRow(); }
    inline int getCol() { return m_map->getCol(); }
    inline int getTime(int row, int col) { return m_map->getTime(row, col); }
    inline int getLandForm(int row, int col) { return m_map->getLandForm(row, col); }


    // 给定坐标位置, 计算所处的行列
    int caculRow(float y);
    int caculCol(float x);
    // 判断一个格子是否可以添加植物
    bool isValidCell(int row, int col);

    int updateMap();

    int renderMap();

    ~MapManager();
};

#endif