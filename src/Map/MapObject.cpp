#include "Map/MapObject.h"
#include <fstream>
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
// 全局单例
#include "Core/CoreVars.h"
#include "Resource/ResVars.h"

MapObject::MapObject()
{}

int MapObject::load(const std::string& file)
{
    std::ifstream map_file(file);
    if (!map_file.is_open()) return -1;
    rapidjson::IStreamWrapper isw(map_file);
    rapidjson::Document doc;
    doc.ParseStream(isw);
    m_bkTexture = ResVars::getInstance().textureRes.getTextureFrom(doc["texture"].GetString());
    int w, h;
    SDL_QueryTexture(m_bkTexture, NULL, NULL, &w, &h);
    m_mapWidthPixel = static_cast<float>(w);
    m_mapHeightPixel = static_cast<float>(h);
    m_leftMargin = doc["left"].GetFloat();
    m_rightMargin = doc["right"].GetFloat();
    m_topMargin = doc["top"].GetFloat();
    m_bottomMargin = doc["bottom"].GetFloat();
    m_rowNum = doc["row"].GetInt();
    m_colNum = doc["col"].GetInt();
    m_cellWidth = (m_rightMargin - m_leftMargin) / m_colNum;
    m_cellHeight = (m_bottomMargin - m_topMargin) / m_rowNum;
    m_mapTemplate.resize(m_rowNum);
    for (auto& r : m_mapTemplate)
    {
        r.resize(m_colNum);
    }
    rapidjson::Value& data_time = doc["time"];
    assert(data_time.IsObject());
    for (int i = 0;i < m_rowNum;i++)
    {
        std::string row_tag = "row" + std::to_string(i);
        assert(data_time[row_tag.c_str()].IsArray());
        for (int j = 0;j < m_colNum;j++)
        {
            m_mapTemplate[i][j].m_time = time_str_map[data_time[row_tag.c_str()][j].GetString()];
        }
    }
    rapidjson::Value& data_landtype = doc["type"];
    assert(data_landtype.IsObject());
    for (int i = 0;i < m_rowNum;i++)
    {
        std::string row_tag = "row" + std::to_string(i);
        assert(data_landtype[row_tag.c_str()].IsArray());
        for (int j = 0;j < m_colNum;j++)
        {
            m_mapTemplate[i][j].m_landForm = landform_str_map[data_landtype[row_tag.c_str()][j].GetString()];
        }
    }

    m_mapRunTime = m_mapTemplate;
    return 0;
}

MapObject::~MapObject()
{}
