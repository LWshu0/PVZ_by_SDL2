#ifndef MAPMANAGER_H
#define MAPMANAGER_H

#include <vector>
#include <memory>

#include "Map/MapInitilizer.h"

class MapManager {
protected:
    // 地图大小
    float m_mapWidthPixel;
    float m_mapHeightPixel;
    // 可种植区域的边界
    float m_leftMargin;
    float m_rightMargin;
    float m_topMargin;
    float m_bottomMargin;
    // 格子大小
    float m_cellWidth;
    float m_cellHeight;
    // 可种植区域的行列数
    int m_rowNum;
    int m_colNum;
    // 二维数组
    std::vector<std::vector<MapNode>> m_mapRunTime;

    std::shared_ptr<MapInitilizer> m_initilizer;
public:
    MapManager();

    int setMap(
        float width,
        float height,
        float left,
        float right,
        float top,
        float bottom,
        std::shared_ptr<MapInitilizer> mapTemplate
    );

    inline void setTime(MapNode::Time time, int row, int col) { m_mapRunTime[row][col].m_time = time; }
    inline void setLandForm(MapNode::LandForm landform, int row, int col) { m_mapRunTime[row][col].m_landForm = landform; }

    inline float getWidth() { return m_mapWidthPixel; }
    inline float getHeight() { return m_mapHeightPixel; }
    inline float getLeftMargin() { return m_leftMargin; }
    inline float getTopMargin() { return m_topMargin; }
    inline float getCellWidth() { return m_cellWidth; }
    inline float getCellHeight() { return m_cellHeight; }
    inline int getRow() { return m_rowNum; }
    inline int getCol() { return m_colNum; }
    inline int getTime(int row, int col) { return m_mapRunTime[row][col].m_time; }
    inline int getLandForm(int row, int col) { return m_mapRunTime[row][col].m_landForm; }

    ~MapManager();
};

#endif