#include "Manager/TaskManager.h"
#include "Manager/ZombieManager.h"
#include "Manager/MapManager.h"

TaskManager::TaskManager() :
    m_taskPointer(-1)
{}

int TaskManager::initilizeManagers(
    std::shared_ptr<MapManager> mapManager,
    std::shared_ptr<ZombieManager> zombieManager
)
{
    m_mapManager = mapManager;
    m_zombieManager = zombieManager;
    return 0;
}

int TaskManager::loadTask(const std::string& file_path)
{
    m_taskRecord.clear();
    XmlLoader loader;
    if (loader.Import(file_path) == 0)
    {
        m_taskPointer = 0;
        auto children = loader.m_root->getChildren();
        for (auto ptr : children)
        {
            TaskEvent event;
            std::string node_name;
            node_name = ptr->getName();
            if ("group" == node_name)
            {
                addGroupEvent(ptr);
            }
            else if ("task" == node_name)
            {
                addTaskEvent(ptr);
            }
        }
    }
    else
    {
        m_taskPointer = -1;
    }
    return m_taskPointer;
}

int TaskManager::updateTask()
{
    // task 错误
    if (m_taskPointer < 0) return -1;
    // 侦测直到等待事件触发
    uint64_t delta_ms = GlobalVars::getInstance().timer.getDeltaTime();    // 剩余时间
    while (m_taskPointer < m_taskRecord.size())
    {
        // prewait
        if (m_taskRecord[m_taskPointer].m_prewaitMilliSecond > delta_ms)
        {
            m_taskRecord[m_taskPointer].m_prewaitMilliSecond -= delta_ms;
            return 0;
        }
        else
        {
            delta_ms -= m_taskRecord[m_taskPointer].m_prewaitMilliSecond;
            m_taskRecord[m_taskPointer].m_prewaitMilliSecond = 0;
        }
        // handle event
        for (int i = 0;i < m_taskRecord[m_taskPointer].m_zombieNumber;i++)
        {
            int row_idx = m_taskRecord[m_taskPointer].m_rowIdx;
            int col_idx = m_taskRecord[m_taskPointer].m_colIdx;
            if (row_idx < 0) row_idx = rand() % m_mapManager->getRow();
            if (col_idx < 0) col_idx = m_mapManager->getCol();
            m_zombieManager->addZombie(m_taskRecord[m_taskPointer].m_zombieType, row_idx, col_idx);
        }
        m_taskPointer++;
    }
    return 1;
}

void TaskManager::releaseManagers()
{
    m_mapManager = nullptr;
    m_zombieManager = nullptr;
}

TaskManager::~TaskManager()
{}

bool TaskManager::isValidEvent(TaskEvent& event)
{
    /* 事件不能为空
        当等待时间全部为 0 且不增加新的僵尸时, 即为空事件
    */
    if (event.m_prewaitMilliSecond == 0
        && (event.m_zombieType == ZombieType::MaxZombieType
            || event.m_zombieNumber <= 0)) return false;
    /* 行数不能超过地图限制的范围 */
    if (event.m_rowIdx >= m_mapManager->getRow()) return false;
    // 否则
    return true;
}

void TaskManager::addTaskEvent(XmlNodePtr ptr)
{
    TaskEvent event;
    std::string content;
    content = ptr->getAttr("prewait");
    if (content != "") event.m_prewaitMilliSecond = std::stoll(content);
    content = ptr->getAttr("zombietype");
    if (content != "")
    {
        int zombie_type = std::stoi(content);
        if (zombie_type < ZombieType::MaxZombieType && zombie_type >= 0)
        {
            event.m_zombieType = static_cast<ZombieType>(zombie_type);
        }
    }
    content = ptr->getAttr("number");
    if (content != "") event.m_zombieNumber = std::stoll(content);
    content = ptr->getAttr("rowidx");
    if (content != "") event.m_rowIdx = std::stoll(content);
    content = ptr->getAttr("colidx");
    if (content != "") event.m_colIdx = std::stoll(content);
    // check & push
    if (isValidEvent(event))
    {
        m_taskRecord.push_back(event);
    }
}

void TaskManager::addGroupEvent(XmlNodePtr ptr)
{
    // 重复次数
    std::string content = ptr->getAttr("repeat");
    if ("" == content) return;
    int repeat_times = std::stoi(content);
    // 重复间隔
    content = ptr->getAttr("septime");
    if ("" == content) return;
    int sep_ms = std::stoi(content);
    TaskEvent wait_event;
    wait_event.m_prewaitMilliSecond = sep_ms;
    // handle
    for (int i = 0;i < repeat_times;i++)
    {
        auto children = ptr->getChildren();
        for (auto child : children)
        {
            if ("task" == child->getName())
            {
                addTaskEvent(child);
            }
        }
        if (i < repeat_times - 1)
        {
            m_taskRecord.push_back(wait_event);
        }
    }
}