#include "Manager/ZombieManager.h"
#include "Manager/MapManager.h"
#include "Manager/ProductManager.h"
#include "Manager/PlantManager.h"
#include "Core/GlobalVars.h"

ZombieManager::ZombieManager()
{
    m_zombieTemplate.resize(ZombieType::MaxZombieType);
    m_animLoader.resize(ZombieType::MaxZombieType);
    // 普通僵尸
    m_animLoader[ZombieType::ZombieNormal] = std::make_shared<AnimLoader>("reanim/Zombie.reanim");
    m_zombieTemplate[ZombieType::ZombieNormal] = std::make_shared<Zombie>(m_animLoader[ZombieType::ZombieNormal], SDL_FPoint{ 0.0f, 0.0f });
}

int ZombieManager::initilizeZombie()
{
    m_zombies.clear();
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
    float root_x = GlobalVars::getInstance().mapManager->getLeftMargin() + col * GlobalVars::getInstance().mapManager->getCellWidth();
    float root_y = GlobalVars::getInstance().mapManager->getTopMargin() + row * GlobalVars::getInstance().mapManager->getCellHeight();
    root_x += GlobalVars::getInstance().mapManager->getCellWidth() / 2;
    root_y += GlobalVars::getInstance().mapManager->getCellHeight() * 0.8;
    m_zombies.push_front(m_zombieTemplate[type]->cloneZombie(SDL_FPoint{ root_x, root_y }));
    return 0;
}

bool ZombieManager::hasZombieBetween(int row, float left_x, float right_x)
{
    for (auto& ptr : m_zombies)
    {
        int rowIdx = GlobalVars::getInstance().mapManager->caculRow(ptr->m_aabb.y + ptr->m_aabb.h);
        if (rowIdx == row && (ptr->m_aabb.x >= left_x && ptr->m_aabb.x <= right_x))
        {
            return true;
        }
    }
    return false;
}

bool ZombieManager::hasZombieInAttackRange(std::shared_ptr<PlantObject> plant)
{
    for (auto& ptr : m_zombies)
    {
        if (plant->inAttackRange(ptr->m_aabb)) return true;
    }
    return false;
}

bool ZombieManager::hasZombieInHouse()
{
    float house_margin = GlobalVars::getInstance().camera.getClickX(0);
    for (auto& ptr : m_zombies)
    {
        if (ptr->m_aabb.x + ptr->m_aabb.w < house_margin)
        {
            return true;
        }
    }
    return false;
}

int ZombieManager::updateZombie()
{
    for (auto iter = m_zombies.begin();iter != m_zombies.end();)
    {
        // 移动
        (*iter)->updateZombie();
        // 碰撞检测
        int dam = GlobalVars::getInstance().productManager->calculateDamage((*iter));
        (*iter)->damage(dam);
        if ((*iter)->isDead())
        {
            iter = m_zombies.erase(iter);
        }
        else
        {
            iter++;
        }
    }
    return 0;
}

int ZombieManager::attackPlants()
{
    for (auto& ptr : m_zombies)
    {
        int row = GlobalVars::getInstance().mapManager->caculRow(ptr->m_aabb.y + ptr->m_aabb.h);
        int col = GlobalVars::getInstance().mapManager->caculCol(ptr->m_aabb.x);
        if (GlobalVars::getInstance().plantManager->collisionPlant(ptr, row, col))
        {
            ptr->setZombieState(ZombieState::Zombie_ATTACK);
            int dam = ptr->attack();
            GlobalVars::getInstance().plantManager->doDamage(row, col, dam);
        }
        else
        {
            ptr->setZombieState(ZombieState::Zombie_WALK);
        }
    }
    return 0;
}

int ZombieManager::renderZombie()
{
    for (auto& ptr : m_zombies)
    {
        ptr->render();
    }
    return 0;
}

ZombieManager::~ZombieManager()
{}