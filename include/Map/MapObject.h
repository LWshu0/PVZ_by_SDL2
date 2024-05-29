#ifndef MAPOBJECT_H
#define MAPOBJECT_H

#include <vector>
#include <map>
#include <string>
#include <memory>
#include "SDL.h"

struct MapNode {
    enum Time {
        DAY,
        EVE
    } m_time;
    enum LandForm {
        NAKE,
        GRASS
    } m_landForm;

};

static std::map<std::string, MapNode::Time> time_str_map{
    {"day", MapNode::Time::DAY },
    {"eve", MapNode::Time::EVE}
};

static std::map<std::string, MapNode::LandForm> landform_str_map{
    {"nake", MapNode::LandForm::NAKE},
    {"grass", MapNode::LandForm::GRASS}
};

class MapObject {
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
    // 地图背景
    SDL_Texture* m_bkTexture;
    // 地图数据
    std::vector<std::vector<MapNode>> m_mapTemplate;
    std::vector<std::vector<MapNode>> m_mapRunTime;
public:
    MapObject();
    int load(const std::string& file);
    inline void setTime(MapNode::Time time, int row, int col) { m_mapRunTime[row][col].m_time = time; }
    inline void setLandForm(MapNode::LandForm landform, int row, int col) { m_mapRunTime[row][col].m_landForm = landform; }

    inline float getWidth() { return m_mapWidthPixel; }
    inline float getHeight() { return m_mapHeightPixel; }
    inline float getLeftMargin() { return m_leftMargin; }
    inline float getRightMargin() { return m_rightMargin; }
    inline float getTopMargin() { return m_topMargin; }
    inline float getBottomMargin() { return m_bottomMargin; }
    inline float getCellWidth() { return m_cellWidth; }
    inline float getCellHeight() { return m_cellHeight; }
    inline int getRow() { return m_rowNum; }
    inline int getCol() { return m_colNum; }
    inline int getTime(int row, int col) { return m_mapRunTime[row][col].m_time; }
    inline int getLandForm(int row, int col) { return m_mapRunTime[row][col].m_landForm; }


    virtual int update() = 0;
    virtual int render() = 0;
    ~MapObject();
};

#endif