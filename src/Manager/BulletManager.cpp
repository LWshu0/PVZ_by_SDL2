#include "Manager/BulletManager.h"

BulletManager::BulletManager(
    SDL_Renderer* renderer,
    std::shared_ptr<TextureRes> texture_res,
    std::shared_ptr<Camera> camera,
    int maxBullet
) :
    m_renderer(renderer),
    m_textureRes(texture_res),
    m_camera(camera),
    m_maxBulletNum(maxBullet)
{
    // 初始化模板数组
    m_bulletTemplate.resize(BulletType::MaxBulletType);
    // 普通豌豆 初始化
    m_bulletTemplate[BulletType::BulletPea] = std::make_shared<Pea>(
        m_renderer,
        m_textureRes->getTextureFrom("images/ProjectilePea.png"),
        m_camera, 0.0f, 0.0f);
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
            ptr = m_bulletTemplate[type]->createBullet(x, y);
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
