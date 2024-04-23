#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <vector>
#include <memory>
#include "Core/Timer.h"
#include "Core/XmlLoader.h"
#include "Zombies/ZombieObject.h"

class MapManager;
class ZombieManager;

struct TaskEvent
{
    uint64_t m_prewaitMilliSecond = 0;                      // 该事件执行之前等待的时间
    ZombieType m_zombieType = ZombieType::MaxZombieType;    // 添加的僵尸种类
    unsigned int m_zombieNumber = 0;                        // 添加的僵尸数量
    int m_rowIdx = -1;                                      // 僵尸所在的行号 从 0 开始, -1 代表随机一行
    int m_colIdx = -1;                                      // 僵尸所在的列号 从 0 开始, -1 代表最右侧
};


class TaskManager {
protected:
    std::shared_ptr<Timer> m_timer;         // 游戏时钟
    int m_taskPointer;                      // 当前所在的记录下标
    std::vector<TaskEvent> m_taskRecord;    // 关卡记录

    // 关联 manager
    std::shared_ptr<MapManager> m_mapManager;
    std::shared_ptr<ZombieManager> m_zombieManager;
public:
    TaskManager(std::shared_ptr<Timer> timer);

    int initilizeManagers(
        std::shared_ptr<MapManager> mapManager,
        std::shared_ptr<ZombieManager> zombieManager
    );

    int loadTask(const std::string& file_path);

    /**
     * @return int 1 代表关卡结束, 0 代表正在进行关卡, -1 代表出错
     */
    int updateTask();

    void releaseManagers();

    ~TaskManager();

protected:

    bool isValidEvent(TaskEvent& event);
    // 解析一个 task 标签事件
    void addTaskEvent(XmlNodePtr ptr);
    // 解析一个 group 标签事件
    void addGroupEvent(XmlNodePtr ptr);
};

#endif