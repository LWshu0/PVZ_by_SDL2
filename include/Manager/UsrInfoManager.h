#ifndef USRINFOMANAGER_H
#define USRINFOMANAGER_H

#include "Plants/PlantObject.h"
#include "Manager/Managers.h"

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

public:
    UsrInfoManager();

    int read();

    inline int getRoundNum() { return m_roundNum; }
    inline int getTaskPre() { return m_taskIdxPre; }
    inline int getTaskPost() { return m_taskIdxPost; }
    inline int getUnlockSlotNum() { return m_unlockSlotNum; }
    inline int getUnlockPlantNum() { return m_unlockPlantNum; }

    void nextTask();

    void unlockPlant(PlantType plant_type);
    bool isUnlockPlant(int plant_type);

    ~UsrInfoManager();
};

#endif