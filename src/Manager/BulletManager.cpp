#include "Manager/BulletManager.h"

BulletManager::BulletManager(
    int maxBullet
) :
    m_maxBulletNum(maxBullet)
{
    // 初始化模板数组
    m_bulletTemplate.resize(BulletType::MaxBulletType);
    // 普通豌豆 初始化
    m_bulletTemplate[BulletType::BulletPea] = std::make_shared<Pea>(
        GlobalVars::getInstance().textureRes.getTextureFrom("images/ProjectilePea.png"),
        GlobalVars::getInstance().textureRes.getTextureFrom("images/plantshadow.png"),
        0.0f, 0.0f);
    m_bulletTemplate[BulletType::BulletPea]->setMotion(std::make_shared<MotionSpeed>(200.0f, 0.0f));
    // 
    // 待添加新的子弹 ....
    //

    // 初始化子弹数组
    m_bullets.resize(m_maxBulletNum);
    for (int i = 0;i < m_maxBulletNum;i++)
    {
        m_bullets[i] = nullptr;
    }
}

int BulletManager::addBullet(BulletType type, float x, float y)
{
    if (type == BulletType::MaxBulletType) return -1;
    for (auto& ptr : m_bullets)
    {
        if (nullptr == ptr)
        {
            ptr = m_bulletTemplate[type]->cloneBullet(x, y);
            return 0;
        }
    }
    return -1;
}

int BulletManager::collisionBullet(GameObject& other)
{
    int rt_value = 0;
    for (auto& ptr : m_bullets)
    {
        if (nullptr == ptr) continue;
        if (ptr->collision(other))
        {
            rt_value += ptr->getDamage();
            ptr = nullptr;
        }
    }
    return rt_value;
}

int BulletManager::collisionBullet(std::shared_ptr<GameObject> other)
{
    int rt_value = 0;
    for (auto& ptr : m_bullets)
    {
        if (nullptr == ptr) continue;
        if (ptr->collision(other))
        {
            rt_value += ptr->getDamage();
            ptr = nullptr;
        }
    }
    return rt_value;
}

int BulletManager::updateBullets()
{
    for (auto& ptr : m_bullets)
    {
        if (nullptr == ptr) continue;
        ptr->updateBullet();
        // 超出屏幕范围子弹检查
        if (ptr->m_aabb.x > GlobalVars::getInstance().camera.getRight()
            || ptr->m_aabb.x + ptr->m_aabb.w < GlobalVars::getInstance().camera.getLeft()
            || ptr->m_aabb.y > GlobalVars::getInstance().camera.getBottom()
            || ptr->m_aabb.y + ptr->m_aabb.h < GlobalVars::getInstance().camera.getTop())
        {
            ptr = nullptr;
            // SDL_Log("remove one\n");
        }
    }
    return 0;
}

int BulletManager::renderBullets()
{
    for (auto& ptr : m_bullets)
    {
        if (nullptr == ptr) continue;
        ptr->render();
    }
    return 0;
}

int BulletManager::clearBullets()
{
    for (int i = 0;i < m_maxBulletNum;i++)
    {
        m_bullets[i] = nullptr;
    }
    return 0;
}

BulletManager::~BulletManager()
{}
