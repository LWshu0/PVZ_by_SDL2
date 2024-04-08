#include "Map/GrassDayOneLine.h"

GrassDayOneLine::GrassDayOneLine() :
    MapInitilizer()
{

}

int GrassDayOneLine::initilizeMapTemplate()
{
    if (m_isInit) return 0;
    m_isInit = true;
    m_rowNum = 5;
    m_colNum = 9;
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
    return 0;
}

GrassDayOneLine::~GrassDayOneLine()
{}