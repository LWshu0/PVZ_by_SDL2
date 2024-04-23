#include "Manager/ZombieManager.h"
#include "Manager/MapManager.h"
#include "Manager/BulletManager.h"
#include "Manager/PlantManager.h"

ZombieManager::ZombieManager(
    SDL_Renderer* renderer,
    std::shared_ptr<TextureRes> res,
    std::shared_ptr<Camera> camera,
    std::shared_ptr<Timer> timer
) :
    m_renderer(renderer),
    m_textureRes(res),
    m_camera(camera),
    m_timer(timer),
    m_mapManager(nullptr),
    m_bulletManager(nullptr),
    m_plantManager(nullptr)
{
    m_zombies.resize(10);
    m_zombieTemplate.resize(ZombieType::MaxZombieType);
    m_animLoader.resize(ZombieType::MaxZombieType);
    // 普通僵尸
    m_animLoader[ZombieType::ZombieNormal] = std::make_shared<AnimLoader>("reanim/Zombie.reanim", m_renderer, m_textureRes);
    m_zombieTemplate[ZombieType::ZombieNormal] = std::make_shared<Zombie>(m_animLoader[ZombieType::ZombieNormal], m_camera, SDL_FPoint{ 0.0f, 0.0f });
}

int ZombieManager::initilizeManagers(
    std::shared_ptr<MapManager> mapManager,
    std::shared_ptr<BulletManager> bulletManager,
    std::shared_ptr<PlantManager> plantManager
)
{
    m_mapManager = mapManager;
    m_bulletManager = bulletManager;
    m_plantManager = plantManager;
    return 0;
}

int ZombieManager::initilizeZombie()
{
    m_zombies.clear();
    m_zombies.resize(10);
    return 0;
}

int ZombieManager::addZombie(ZombieType type, int row, int col)
{
    if (row < 0 || row >= m_mapManager->getRow()
        || col < 0  // 可从屏幕外起始
        || ZombieType::MaxZombieType == type)
    {
        return -1;
    }
    for (int i = 0;i < m_zombies.size();i++)
    {
        if (nullptr != m_zombies[i]) continue;
        float root_x = m_mapManager->getLeftMargin() + col * m_mapManager->getCellWidth();
        float root_y = m_mapManager->getTopMargin() + row * m_mapManager->getCellHeight();
        root_x += m_mapManager->getCellWidth() / 2;
        root_y += m_mapManager->getCellHeight() * 0.8;
        m_zombies[i] = m_zombieTemplate[type]->cloneZombie(SDL_FPoint{ root_x, root_y });
        return 0;
    }
    return -1;
}

bool ZombieManager::hasZombieBetween(int row, float left_x, float right_x)
{
    for (int i = 0;i < m_zombies.size();i++)
    {
        if (nullptr == m_zombies[i]) continue;
        int rowIdx = m_mapManager->caculRow(m_zombies[i]->m_aabb.y + m_zombies[i]->m_aabb.h);
        if (rowIdx == row && (m_zombies[i]->m_aabb.x >= left_x && m_zombies[i]->m_aabb.x <= right_x))
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
        m_zombies[i]->updateZombie(m_timer);
        // 碰撞检测
        int dam = m_bulletManager->collisionBullet(m_zombies[i]);
        m_zombies[i]->damage(dam);
        if (m_zombies[i]->isDead())
        {
            m_zombies[i] = nullptr;
        }
    }
    return 0;
}

int ZombieManager::attackPlants()
{
    for (int i = 0;i < m_zombies.size();i++)
    {
        if (nullptr == m_zombies[i]) continue;
        int row = m_mapManager->caculRow(m_zombies[i]->m_aabb.y + m_zombies[i]->m_aabb.h);
        int col = m_mapManager->caculCol(m_zombies[i]->m_aabb.x);
        if (m_plantManager->collisionPlant(m_zombies[i], row, col))
        {
            m_zombies[i]->changeZombieState(ZombieState::Zombie_ATTACK, m_timer);
            int dam = m_zombies[i]->attack(m_timer);
            m_plantManager->doDamage(row, col, dam);
        }
        else
        {
            m_zombies[i]->changeZombieState(ZombieState::Zombie_WALK, m_timer);
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

void ZombieManager::releaseManagers()
{
    m_mapManager = nullptr;
    m_bulletManager = nullptr;
    m_plantManager = nullptr;
}

ZombieManager::~ZombieManager()
{}

int ZombieManager::changeAllTo(ZombieState state)
{
    for (int i = 0;i < m_zombies.size();i++)
    {
        if (nullptr == m_zombies[i]) continue;
        if (-1 == m_zombies[i]->changeZombieState(state, m_timer))
        {
            SDL_Log("change failed\n");
        }
    }
    return 0;
}