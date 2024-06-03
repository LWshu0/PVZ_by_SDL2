#include "Manager/UsrInfoManager.h"

UsrInfoManager::UsrInfoManager()
{}

int UsrInfoManager::read()
{
    std::ifstream map_file("resource/usr.json");
    if (!map_file.is_open()) return -1;
    rapidjson::IStreamWrapper isw(map_file);
    m_doc.ParseStream(isw);
    m_roundNum = m_doc["round"].GetInt();
    m_taskIdxPre = m_doc["task"]["pre"].GetInt();
    m_taskIdxPost = m_doc["task"]["post"].GetInt();
    m_unlockSlotNum = m_doc["unlock_slot"].GetInt();

    memset(m_unlockPlants, 0, sizeof(m_unlockPlants));
    m_unlockPlantNum = 0;
    rapidjson::Value& unlock_plants_array = m_doc["unlock_plants"];
    for (rapidjson::Value::ConstValueIterator itr = unlock_plants_array.Begin(); itr != unlock_plants_array.End(); ++itr)
    {
        int plant_idx = itr->GetInt();
        if (0 <= plant_idx && plant_idx < PlantType::MaxPlantType)
        {
            m_unlockPlants[plant_idx] = true;
            m_unlockPlantNum += 1;
        }
    }
    map_file.close();
    return 0;
}

int UsrInfoManager::save()
{
    m_doc["round"].SetInt(m_roundNum);
    m_doc["task"]["pre"].SetInt(m_taskIdxPre);
    m_doc["task"]["post"].SetInt(m_taskIdxPost);
    m_doc["unlock_slot"].SetInt(m_unlockSlotNum);
    rapidjson::Value a(rapidjson::kArrayType);
    rapidjson::Document::AllocatorType& allocator = m_doc.GetAllocator();
    m_doc["unlock_plants"].Reserve(PlantType::MaxPlantType, allocator);
    m_doc["unlock_plants"].Clear();
    for (int i = 0; i < PlantType::MaxPlantType; i++)
    {
        if (m_unlockPlants[i])
        {
            m_doc["unlock_plants"].PushBack(i, allocator);
        }
    }
    std::ofstream map_file("resource/usr.json");
    if (!map_file.is_open()) return -1;
    rapidjson::OStreamWrapper osw(map_file);
    rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
    m_doc.Accept(writer);
    map_file.close();
    return 0;
}

void UsrInfoManager::nextTask()
{
    m_taskIdxPost += 1;
    if (m_taskIdxPost > 10)
    {
        m_taskIdxPost = 1;
        m_taskIdxPre += 1;
        if (m_taskIdxPre > 10)
        {
            m_taskIdxPre = 1;
            m_roundNum += 1;
        }
    }
}

void UsrInfoManager::unlockPlant(int plant_type)
{
    if (plant_type != PlantType::MaxPlantType)
    {
        m_unlockPlants[plant_type] = true;
    }
}

bool UsrInfoManager::isUnlockPlant(int plant)
{
    if (0 <= plant && plant < PlantType::MaxPlantType) return m_unlockPlants[plant];
    else return false;
}

UsrInfoManager::~UsrInfoManager()
{}
