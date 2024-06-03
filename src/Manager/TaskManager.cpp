#include "Manager/TaskManager.h"
#include "Manager/ZombieManager.h"
#include "Manager/MapManager.h"
#include "Manager/UsrInfoManager.h"
// 全局单例
#include "Core/CoreVars.h"
#include "Resource/ResVars.h"

TaskManager::TaskManager() :
    m_taskPointer(-1)
{
    m_progressBarBK = ResVars::getInstance().textureRes.getTextureFrom("images/FlagMeter.png");
    m_progressBarTag = ResVars::getInstance().textureRes.getTextureFrom("images/FlagMeterLevelProgress.png");
    m_progressBarFlag = ResVars::getInstance().textureRes.getTextureFrom("images/FlagMeterParts.png");
    int w, h;
    // bk
    SDL_QueryTexture(m_progressBarBK, NULL, NULL, &w, &h);
    m_fullBarSrc = SDL_Rect{ 0, h / 2, w, h / 2 };
    m_emptyBarSrc = SDL_Rect{ 0, 0, w, h / 2 };
    m_barDst = SDL_Rect{
        (int)CoreVars::getInstance().window_width - w - 20,
        (int)CoreVars::getInstance().window_height - 30,
        w,
        h / 2
    };
    // tag
    SDL_QueryTexture(m_progressBarTag, NULL, NULL, &w, &h);
    m_tagDst = SDL_Rect{ m_barDst.x + m_barDst.w / 2, m_barDst.y + m_barDst.h, w, h };
    // parts
    SDL_QueryTexture(m_progressBarFlag, NULL, NULL, &w, &h);
    m_headSrc = SDL_Rect{ 0, 0, w / 3, h };
    m_poleSrc = SDL_Rect{ w / 3, 0, w / 3, h };
    m_flagSrc = SDL_Rect{ w / 3 * 2, 0, w / 3, h };

    m_nextWave = ResVars::getInstance().textureRes.getTextureFrom("images/APPROACHING.png");
    m_finalWave = ResVars::getInstance().textureRes.getTextureFrom("images/FinalWave.png");

}

int TaskManager::loadTask(const std::string& file_path)
{
    m_nextContinueTime = 0;
    m_lastContinueTime = 0;
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
            else if ("wave" == node_name)
            {
                addWaveEvent(ptr);
            }
        }
    }
    else
    {
        m_taskPointer = -1;
    }
    return m_taskPointer;
}

int TaskManager::loadTask()
{
    int pre = Managers::getInstance().usrinfoManager->getTaskPre();
    int post = Managers::getInstance().usrinfoManager->getTaskPost();
    int round = Managers::getInstance().usrinfoManager->getRoundNum();
    std::string task_file_name = std::to_string(pre) + "-" + std::to_string(post) + "-";
    if (round >= 1) task_file_name += "n";
    else task_file_name += "1";
    task_file_name += ".xml";
    return loadTask("task/" + task_file_name);
}

int TaskManager::updateTask()
{
    // task 错误
    if (m_taskPointer < 0) return -1;
    // 侦测直到等待事件触发
    uint64_t delta_ms = CoreVars::getInstance().timer.getDeltaTime();    // 剩余时间
    while (m_taskPointer < m_taskRecord.size())
    {
        // prewait
        if (m_taskRecord[m_taskPointer].m_prewaitMilliSecond > delta_ms)
        {
            m_taskRecord[m_taskPointer].m_prewaitMilliSecond -= delta_ms;
            break;
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
            if (row_idx < 0) row_idx = rand() % Managers::getInstance().mapManager->getRow();
            if (col_idx < 0) col_idx = Managers::getInstance().mapManager->getCol();
            Managers::getInstance().zombieManager->addZombie(m_taskRecord[m_taskPointer].m_zombieType, row_idx, col_idx);
        }
        if (isWaveStart(m_taskPointer + 1))
        {
            m_nextContinueTime = 3000;
        }
        if (isLastWaveStart(m_taskPointer))
        {
            m_lastContinueTime = 3000;
        }
        m_taskPointer++;
    }
    if (m_nextContinueTime > 0)
    {
        m_nextContinueTime -= CoreVars::getInstance().timer.getDeltaTime();
    }
    else
    {
        if (m_lastContinueTime > 0)
        {
            m_lastContinueTime -= CoreVars::getInstance().timer.getDeltaTime();
        }
    }

    return 0;
}

int TaskManager::renderTask()
{
    // 进度条
    int full_length = getFullLength();
    SDL_Rect empty_dst{
        m_barDst.x, m_barDst.y,
        m_barDst.w - full_length,
        m_barDst.h
    };
    SDL_Rect empty_src{
        m_emptyBarSrc.x, m_emptyBarSrc.y,
        m_barDst.w - full_length,
        m_emptyBarSrc.h
    };
    SDL_RenderCopy(CoreVars::getInstance().renderer, m_progressBarBK, &empty_src, &empty_dst);

    SDL_Rect full_rect{
        empty_dst.x + empty_dst.w, m_barDst.y,
        full_length,
        m_barDst.h
    };
    SDL_Rect full_src{
        m_fullBarSrc.x + empty_dst.w, m_fullBarSrc.y,
        full_length,
        m_fullBarSrc.h
    };
    SDL_RenderCopy(CoreVars::getInstance().renderer, m_progressBarBK, &full_src, &full_rect);

    // flag
    int flag_sep = 145 / m_waveRecord.size();
    for (int i = 0;i < m_waveRecord.size();i++)
    {
        // raise
        if (m_taskPointer > m_waveRecord[i].m_startTaskIdx)
        {
            SDL_Rect pole_rect{
                m_barDst.x + flag_sep * ((int)m_waveRecord.size() - i - 1) + 7,
                m_barDst.y - 7,
                m_poleSrc.w,
                m_poleSrc.h
            };
            SDL_RenderCopy(CoreVars::getInstance().renderer, m_progressBarFlag, &m_poleSrc, &pole_rect);
            SDL_Rect flag_rect{
                m_barDst.x + flag_sep * ((int)m_waveRecord.size() - i - 1) + 7,
                m_barDst.y - 20,
                m_flagSrc.w,
                m_flagSrc.h
            };
            SDL_RenderCopy(CoreVars::getInstance().renderer, m_progressBarFlag, &m_flagSrc, &flag_rect);
        }
        else
        {
            SDL_Rect flag_rect{
                m_barDst.x + flag_sep * ((int)m_waveRecord.size() - i - 1) + 7,
                m_barDst.y - 3,
                m_flagSrc.w,
                m_flagSrc.h
            };
            SDL_RenderCopy(CoreVars::getInstance().renderer, m_progressBarFlag, &m_flagSrc, &flag_rect);
        }
    }

    // tag
    SDL_Rect tag_dst{
        m_barDst.x + 36, m_barDst.y + 15,
        86, 11
    };
    SDL_RenderCopy(CoreVars::getInstance().renderer, m_progressBarTag, NULL, &tag_dst);

    // zombie head
    SDL_Rect head_dst = SDL_Rect{
        full_rect.x - m_headSrc.w / 2,
        full_rect.y,
        m_headSrc.w,
        m_headSrc.h
    };
    SDL_RenderCopy(CoreVars::getInstance().renderer, m_progressBarFlag, &m_headSrc, &head_dst);

    if (m_nextContinueTime > 0)
    {
        SDL_Rect next_rect{
        154, 250,
        492, 80
        };
        SDL_RenderCopy(CoreVars::getInstance().renderer, m_nextWave, NULL, &next_rect);
    }
    else
    {
        if (m_lastContinueTime > 0)
        {
            SDL_Rect last_rect{
            230, 250,
            341, 80
            };
            SDL_RenderCopy(CoreVars::getInstance().renderer, m_finalWave, NULL, &last_rect);
        }
    }
    return 0;
}

TaskManager::~TaskManager()
{}

bool TaskManager::isWaveStart(int idx)
{
    for (int i = 0;i < m_waveRecord.size();i++)
    {
        if (idx == m_waveRecord[i].m_startTaskIdx)
        {
            return true;
        }
    }
    return false;
}

bool TaskManager::isLastWaveStart(int idx)
{
    return idx == m_waveRecord[m_waveRecord.size() - 1].m_startTaskIdx;
}

int TaskManager::getFullLength()
{
    // 波次
    float base = 6;
    float sum = 0;
    float flag_sep = 145 / m_waveRecord.size();

    int i = 0;
    for (i = 0;i <= m_waveRecord.size();i++)
    {
        if (m_taskPointer > m_waveRecord[i].m_startTaskIdx) sum += flag_sep;
        else break;
    }
    if (i == 0)
    {
        return (int)(6 + flag_sep * m_taskPointer / m_waveRecord[i].m_startTaskIdx);
    }
    if (i >= m_waveRecord.size())
    {
        return 151;
    }
    else
    {
        int total = m_waveRecord[i].m_startTaskIdx - m_waveRecord[i - 1].m_startTaskIdx;
        int pass = m_taskPointer - m_waveRecord[i - 1].m_startTaskIdx;
        return (int)(6 + sum + flag_sep * pass / total);
    }
}

bool TaskManager::isValidEvent(TaskEvent& event)
{
    /* 事件不能为空
        当等待时间全部为 0 且不增加新的僵尸时, 即为空事件
    */
    if (event.m_prewaitMilliSecond == 0
        && (event.m_zombieType == ZombieType::MaxZombieType
            || event.m_zombieNumber <= 0)) return false;
    /* 行数不能超过地图限制的范围 */
    if (event.m_rowIdx >= Managers::getInstance().mapManager->getRow()) return false;
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

void TaskManager::addWaveEvent(XmlNodePtr ptr)
{
    int wave_start = m_taskRecord.size();
    auto children = ptr->getChildren();
    for (auto child : children)
    {
        TaskEvent event;
        std::string node_name;
        node_name = child->getName();
        if ("group" == node_name)
        {
            addGroupEvent(child);
        }
        else if ("task" == node_name)
        {
            addTaskEvent(child);
        }
        else if ("wave" == node_name)
        {
            // do nothing
        }
    }
    int wave_end = m_taskRecord.size();
    if (wave_end > wave_start)
        m_waveRecord.push_back(WaveEvent{ wave_start, wave_end });
}