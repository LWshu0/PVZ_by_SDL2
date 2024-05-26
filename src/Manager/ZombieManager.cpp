#include "Manager/ZombieManager.h"
#include "Manager/MapManager.h"
#include "Manager/ProductManager.h"
#include "Manager/PlantManager.h"
#include "Core/GlobalVars.h"

ZombieManager::ZombieManager() :
    m_zombieNum(0)
{
    m_zombies.resize(10);
    m_zombieTemplate.resize(ZombieType::MaxZombieType);
    m_animLoader.resize(ZombieType::MaxZombieType);
    // 普通僵尸
    m_animLoader[ZombieType::ZombieNormal] = std::make_shared<AnimLoader>("reanim/Zombie.reanim");
    m_zombieTemplate[ZombieType::ZombieNormal] = std::make_shared<Zombie>(m_animLoader[ZombieType::ZombieNormal], SDL_FPoint{ 0.0f, 0.0f });
}

int ZombieManager::initilizeZombie()
{
    m_zombieNum = 0;
    m_zombies.clear();
    m_zombies.resize(10);
    return 0;
}

int ZombieManager::addZombie(ZombieType type, int row, int col)
{
    if (row < 0 || row >= GlobalVars::getInstance().mapManager->getRow()
        || col < 0  // 不可从屏幕左侧起始, 可从屏幕右侧起始
        || ZombieType::MaxZombieType == type)
    {
        return -1;
    }
    for (int i = 0;i < m_zombies.size();i++)
    {
        if (nullptr != m_zombies[i]) continue;
        float root_x = GlobalVars::getInstance().mapManager->getLeftMargin() + col * GlobalVars::getInstance().mapManager->getCellWidth();
        float root_y = GlobalVars::getInstance().mapManager->getTopMargin() + row * GlobalVars::getInstance().mapManager->getCellHeight();
        root_x += GlobalVars::getInstance().mapManager->getCellWidth() / 2;
        root_y += GlobalVars::getInstance().mapManager->getCellHeight() * 0.8;
        m_zombies[i] = m_zombieTemplate[type]->cloneZombie(SDL_FPoint{ root_x, root_y });
        m_zombieNum += 1;
        return 0;
    }
    return -1;
}

bool ZombieManager::hasZombieBetween(int row, float left_x, float right_x)
{
    for (int i = 0;i < m_zombies.size();i++)
    {
        if (nullptr == m_zombies[i]) continue;
        int rowIdx = GlobalVars::getInstance().mapManager->caculRow(m_zombies[i]->m_aabb.y + m_zombies[i]->m_aabb.h);
        if (rowIdx == row && (m_zombies[i]->m_aabb.x >= left_x && m_zombies[i]->m_aabb.x <= right_x))
        {
            return true;
        }
    }
    return false;
}

bool ZombieManager::hasZombieInAttackRange(std::shared_ptr<PlantObject> plant)
{
    for (int i = 0;i < m_zombies.size();i++)
    {
        if (nullptr == m_zombies[i]) continue;
        if (plant->inAttackRange(m_zombies[i]->m_aabb)) return true;
    }
    return false;
}

bool ZombieManager::hasZombieInHouse()
{
    float house_margin = GlobalVars::getInstance().camera.getClickX(0);
    for (int i = 0;i < m_zombies.size();i++)
    {
        if (nullptr == m_zombies[i]) continue;
        if (m_zombies[i]->m_aabb.x + m_zombies[i]->m_aabb.w < house_margin)
        {
            return true;
        }
    }
    return false;
}

int ZombieManager::updateZombie()
{
    for (int i = 0;i < m_zombies.size();i++)
    {
        if (nullptr == m_zombies[i]) continue;
        // 移动
        m_zombies[i]->updateZombie();
        // 碰撞检测
        int dam = GlobalVars::getInstance().productManager->calculateDamage(m_zombies[i]);
        m_zombies[i]->damage(dam);
        if (m_zombies[i]->isDead())
        {
            m_zombies[i] = nullptr;
            m_zombieNum -= 1;
        }
    }
    return 0;
}

int ZombieManager::attackPlants()
{
    for (int i = 0;i < m_zombies.size();i++)
    {
        if (nullptr == m_zombies[i]) continue;
        int row = GlobalVars::getInstance().mapManager->caculRow(m_zombies[i]->m_aabb.y + m_zombies[i]->m_aabb.h);
        int col = GlobalVars::getInstance().mapManager->caculCol(m_zombies[i]->m_aabb.x);
        if (GlobalVars::getInstance().plantManager->collisionPlant(m_zombies[i], row, col))
        {
            m_zombies[i]->setZombieState(ZombieState::Zombie_ATTACK);
            int dam = m_zombies[i]->attack();
            GlobalVars::getInstance().plantManager->doDamage(row, col, dam);
        }
        else
        {
            m_zombies[i]->setZombieState(ZombieState::Zombie_WALK);
        }
    }
    return 0;
}

int ZombieManager::renderZombie()
{
    for (int i = 0;i < m_zombies.size();i++)
    {
        if (nullptr == m_zombies[i]) continue;
        m_zombies[i]->render();
    }
    return 0;
}

ZombieManager::~ZombieManager()
{}

int ZombieManager::changeAllTo(ZombieState state)
{
    for (int i = 0;i < m_zombies.size();i++)
    {
        if (nullptr == m_zombies[i]) continue;
        if (-1 == m_zombies[i]->setZombieState(state))
        {
            SDL_Log("change failed\n");
        }
    }
    return 0;
}