#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <vector>
#include <memory>
#include "Core/Timer.h"
#include "Resource/tinyxml2.h"
#include "Zombies/ZombieObject.h"

#include "Manager/Managers.h"

struct TaskEvent
{
    uint64_t m_prewaitMilliSecond = 0;                      // 该事件执行之前等待的时间
    ZombieType m_zombieType = ZombieType::MaxZombieType;    // 添加的僵尸种类
    unsigned int m_zombieNumber = 1;                        // 添加的僵尸数量
    int m_rowIdx = -1;                                      // 僵尸所在的行号 从 0 开始, -1 代表随机一行
    int m_colIdx = -1;                                      // 僵尸所在的列号 从 0 开始, -1 代表最右侧
};

struct WaveEvent {
    int m_startTaskIdx;
    int m_endTaskIdx;
};

class TaskManager {
protected:
    int m_rewardPlant;                      // 关卡获胜后的奖励植物
    int m_taskPointer;                      // 当前所在的记录下标
    std::vector<TaskEvent> m_taskRecord;    // 关卡记录
    std::vector<WaveEvent> m_waveRecord;    // 进度条记录

    SDL_Texture* m_progressBarBK;
    SDL_Texture* m_progressBarTag;
    SDL_Texture* m_progressBarFlag;
    SDL_Rect m_fullBarSrc;
    SDL_Rect m_emptyBarSrc;
    SDL_Rect m_barDst;
    SDL_Rect m_tagDst;
    SDL_Rect m_headSrc;
    SDL_Rect m_poleSrc;
    SDL_Rect m_flagSrc;

    SDL_Texture* m_nextWave;
    int64_t m_nextContinueTime;
    SDL_Texture* m_finalWave;
    int64_t m_lastContinueTime;

public:
    TaskManager();

    // 读入一个特定格式的 xml 文件, 其中包含关卡信息
    int loadTask(const std::string& file_path);
    // 从 usrinfoManager 中读取进度
    int loadTask();
    /**
     * @return int 1 代表关卡中所有僵尸出现事件结束, 0 代表正在进行关卡, -1 代表出错
     */
    int updateTask();

    // 渲染关卡进度条
    int renderTask();

    inline int getRewardPlant() { return m_rewardPlant; }
    inline bool isFinish() { return m_taskPointer >= m_taskRecord.size(); }

    ~TaskManager();

protected:
    // task 是否是 wave 的开始
    bool isWaveStart(int idx);
    // 
    bool isLastWaveStart(int idx);
    // 计算进度条填充长度
    int getFullLength();

    bool isValidEvent(TaskEvent& event);
    // 解析一个 task 标签事件
    void addTaskEvent(tinyxml2::XMLElement* ptr);
    // 解析一个 group 标签事件
    void addGroupEvent(tinyxml2::XMLElement* ptr);
    // 解析一个 wave 标签事件
    void addWaveEvent(tinyxml2::XMLElement* ptr);
};

#endif