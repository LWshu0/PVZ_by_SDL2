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
    m_zombies.push_front(m_zombieTemplate[type]->clone(SDL_FPoint{ root_x, root_y }));
    m_zombies.front()->setZombieState(ZombieState::Zombie_WALK);
    return 0;
}

int ZombieManager::setZombieState(GameObject* bullet_object, ZombieState state)
{
    int count = 0;
    for (auto& zombie : m_zombies)
    {
        if (zombie->collision(bullet_object))
        {
            count++;
            zombie->setZombieState(state);
        }
    }
    return count;
}

int ZombieManager::damageZombie(GameObject* bullet_object, int damage, bool single)
{
    int count = 0;
    for (auto& zombie : m_zombies)
    {
        if (!zombie->zeroHP() && zombie->collision(bullet_object))
        {
            count++;
            zombie->damage(damage);
            if (single) return 1;
        }
    }
    return count;
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

bool ZombieManager::hasZombieInAttackRange(PlantObject* plant)
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
        if ((*iter)->canDelete())
        {
            iter = m_zombies.erase(iter);
        }
        else
        {
            // 更新
            (*iter)->update();
            iter++;
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