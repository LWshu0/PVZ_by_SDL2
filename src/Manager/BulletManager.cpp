#include "Manager/BulletManager.h"

BulletManager::BulletManager(SDL_Renderer* renderer, int maxBullet) :
    m_renderer(renderer),
    m_maxBulletNum(maxBullet)
{
    m_bullets.resize(m_maxBulletNum);
    for (int i = 0;i < m_maxBulletNum;i++)
    {
        m_bullets[i] = nullptr;
    }
}

int BulletManager::addBullet(std::shared_ptr<BulletObject> bulletTemplate, float x, float y)
{
    for (auto& ptr : m_bullets)
    {
        if (nullptr == ptr)
        {
            ptr = bulletTemplate->createBullet(x, y);
            return 0;
        }
    }
    return -1;
}

int BulletManager::updateBullets(uint64_t delta_ms)
{
    for (auto& ptr : m_bullets)
    {
        if (nullptr != ptr) ptr->updateBullet(delta_ms);
    }
    return 0;
}

int BulletManager::renderBullets()
{
    for (auto& ptr : m_bullets)
    {
        if (nullptr != ptr)
        {
            ptr->renderBullet();
        }
    }
    return 0;
}

BulletManager::~BulletManager()
{}
