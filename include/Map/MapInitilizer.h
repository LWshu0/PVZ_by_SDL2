#ifndef MAPINITILIZER_H
#define MAPINITILIZER_H

#include <vector>

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

class MapInitilizer {
protected:
    bool m_isInit;
    // 可种植区域的行列数
    int m_rowNum;
    int m_colNum;
    std::vector<std::vector<MapNode>> m_mapTemplate;
public:
    MapInitilizer() :
        m_isInit(false),
        m_rowNum(0),
        m_colNum(0)
    {};

    virtual int initilizeMapTemplate() = 0;

    inline int resetMap(std::vector<std::vector<MapNode>>& map) { map = m_mapTemplate; return 0; }
    inline int resetCell(std::vector<std::vector<MapNode>>& map, int row, int col) { map[row][col] = m_mapTemplate[row][col]; return 0; }

    inline int getRow() { return m_rowNum; }
    inline int getCol() { return m_colNum; }
    inline int getTime(int row, int col) { return m_mapTemplate[row][col].m_time; }
    inline int getLandForm(int row, int col) { return m_mapTemplate[row][col].m_landForm; }

    virtual ~MapInitilizer() {};
};

#endif