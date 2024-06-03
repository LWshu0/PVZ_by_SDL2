#ifndef USRINFOMANAGER_H
#define USRINFOMANAGER_H

#include "Plants/PlantObject.h"
#include "Manager/Managers.h"
#include <fstream>
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/ostreamwrapper.h"
#include <rapidjson/writer.h>

class UsrInfoManager
{
private:
    // 一般模式完成的轮数
    int m_roundNum;
    // 目前所在的关卡 m_taskIdxPre - m_taskIdxPost
    int m_taskIdxPre, m_taskIdxPost;
    // 解锁的卡槽
    int m_unlockSlotNum;
    // 解锁的植物
    bool m_unlockPlants[PlantType::MaxPlantType];
    int m_unlockPlantNum;

    rapidjson::Document m_doc;
public:
    UsrInfoManager();

    int read();
    int save();

    inline int getRoundNum() { return m_roundNum; }
    inline int getTaskPre() { return m_taskIdxPre; }
    inline int getTaskPost() { return m_taskIdxPost; }
    inline int getUnlockSlotNum() { return m_unlockSlotNum; }
    inline int getUnlockPlantNum() { return m_unlockPlantNum; }

    void nextTask();

    void unlockPlant(int plant_type);
    bool isUnlockPlant(int plant_type);

    ~UsrInfoManager();
};

#endif