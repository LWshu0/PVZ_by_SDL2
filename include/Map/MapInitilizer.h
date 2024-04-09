#ifndef MAPINITILIZER_H
#define MAPINITILIZER_H

#include <vector>
#include "Core/TextureRes.h"

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

class MapManager;

class MapInitilizer {
public:
    /*******************************************
    *    按照纹理素材的尺寸进行设置             *
    *    初始化 manager 时根据宽高进行缩放      *
    *******************************************/
    // 纹理素材
    SDL_Texture* m_bkTexture;
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
    std::vector<std::vector<MapNode>> m_mapTemplate;
public:
    MapInitilizer() = default;

    int initilizeMapManager(MapManager* manager, float w, float h);

    inline int resetMap(std::vector<std::vector<MapNode>>& map) { map = m_mapTemplate; return 0; }
    inline int resetCell(std::vector<std::vector<MapNode>>& map, int row, int col) { map[row][col] = m_mapTemplate[row][col]; return 0; }

    inline int getRow() { return m_rowNum; }
    inline int getCol() { return m_colNum; }
    inline int getTime(int row, int col) { return m_mapTemplate[row][col].m_time; }
    inline int getLandForm(int row, int col) { return m_mapTemplate[row][col].m_landForm; }

    virtual ~MapInitilizer() {};
};

#endif