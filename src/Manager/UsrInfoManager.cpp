#include "Manager/UsrInfoManager.h"
#include "Core/GlobalVars.h"
#include <fstream>
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"

UsrInfoManager::UsrInfoManager()
{}

int UsrInfoManager::read()
{
    std::ifstream map_file("resource/usr.json");
    if (!map_file.is_open()) return -1;
    rapidjson::IStreamWrapper isw(map_file);
    rapidjson::Document doc;
    doc.ParseStream(isw);
    m_roundNum = doc["round"].GetInt();
    m_taskIdxPre = doc["task"]["pre"].GetInt();
    m_taskIdxPost = doc["task"]["post"].GetInt();
    m_unlockSlotNum = doc["unlock_slot"].GetInt();

    memset(m_unlockPlants, 0, sizeof(m_unlockPlants));
    rapidjson::Value& unlock_plants_array = doc["unlock_plants"];
    for (rapidjson::Value::ConstValueIterator itr = unlock_plants_array.Begin(); itr != unlock_plants_array.End(); ++itr)
    {
        int plant_idx = itr->GetInt();
        if (0 <= plant_idx && plant_idx < PlantType::MaxPlantType)
            m_unlockPlants[plant_idx] = true;
    }
    return 0;
}

bool UsrInfoManager::isUnlockPlant(int plant)
{
    if (0 <= plant && plant < PlantType::MaxPlantType) return m_unlockPlants[plant];
    else return false;
}

UsrInfoManager::~UsrInfoManager()
{}
