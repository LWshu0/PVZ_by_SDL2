#ifndef MAPMANAGER_H
#define MAPMANAGER_H

#include <vector>
#include <memory>
#include "Core/TextureRes.h"
#include "Core/Camera.h"
#include "Map/MapInitilizer.h"
#include "Map/GrassDayOneLine.h"

enum MapType {
    MapGrassDayOneLine,
    // 此处添加新的地图 ...
    MaxMapType
};

class MapManager {
protected:
    /**********************
    *    初始化后不可变   *
    *    与渲染相关       *
    **********************/
    SDL_Renderer* m_renderer;
    std::shared_ptr<TextureRes> m_textureRes;
    std::shared_ptr<Camera> m_camera;
    std::vector<std::shared_ptr<MapInitilizer>> m_mapTemplate;
    /********************
    *   地图管理变量    *
    ********************/
public:
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
protected:
    // 地图类型
    MapType m_mapType;
    // 二维数组
    std::vector<std::vector<MapNode>> m_mapRunTime;
public:
    MapManager(
        SDL_Renderer* renderer,
        std::shared_ptr<TextureRes> res,
        std::shared_ptr<Camera> camera
    );

    int setMap(
        float width,
        float height,
        MapType type
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

    int caculRow(float y);
    int caculCol(float x);

    int renderMap();

    ~MapManager();
};

#endif