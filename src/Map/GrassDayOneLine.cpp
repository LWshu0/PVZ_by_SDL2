#include "Map/GrassDayOneLine.h"

GrassDayOneLine::GrassDayOneLine(
    std::shared_ptr<TextureRes> res
) :
    MapInitilizer()
{
    m_bkTexture = res->getTextureFrom("images/background1unsodded.jpg");
    int w, h;
    SDL_QueryTexture(m_bkTexture, NULL, NULL, &w, &h);
    m_mapWidthPixel = static_cast<float>(w);
    m_mapHeightPixel = static_cast<float>(h);
    // 手动定位
    m_leftMargin = 261.0f;
    m_rightMargin = 980.0f;
    m_topMargin = 83.0f;
    m_bottomMargin = 576.0f;
    m_rowNum = 5;
    m_colNum = 9;
    m_cellWidth = (m_rightMargin - m_leftMargin) / m_colNum;
    m_cellHeight = (m_bottomMargin - m_topMargin) / m_rowNum;
    m_mapTemplate.resize(m_rowNum);
    for (auto& r : m_mapTemplate)
    {
        r.resize(m_colNum);
    }
    for (auto& block : m_mapTemplate[2])
    {
        block.m_time = MapNode::Time::DAY;
        block.m_landForm = MapNode::LandForm::GRASS;
    }
}

GrassDayOneLine::~GrassDayOneLine()
{}