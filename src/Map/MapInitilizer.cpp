#include "Map/MapInitilizer.h"
#include "Manager/MapManager.h"

int MapInitilizer::initilizeMapManager(MapManager* manager, float w, float h)
{
    float w_factor = w == 0.0f ? 1.0f : w / m_mapWidthPixel;
    float h_factor = h == 0.0f ? 1.0f : h / m_mapHeightPixel;
    manager->m_mapWidthPixel = m_mapWidthPixel * w_factor;
    manager->m_mapHeightPixel = m_mapHeightPixel * h_factor;
    manager->m_leftMargin = m_leftMargin * w_factor;
    manager->m_rightMargin = m_rightMargin * w_factor;
    manager->m_topMargin = m_topMargin * h_factor;
    manager->m_bottomMargin = m_bottomMargin * h_factor;
    manager->m_rowNum = m_rowNum;
    manager->m_colNum = m_colNum;
    manager->m_cellWidth = (manager->m_rightMargin - manager->m_leftMargin) / manager->m_colNum;
    manager->m_cellHeight = (manager->m_bottomMargin - manager->m_topMargin) / manager->m_rowNum;
    manager->m_bkTexture = m_bkTexture;
    return 0;
}