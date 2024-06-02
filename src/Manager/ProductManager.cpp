#include "Manager/ProductManager.h"
#include "Manager/MapManager.h"
// 全局单例
#include "Core/CoreVars.h"
#include "Resource/ResVars.h"

ProductManager::ProductManager(
    int maxCollection,
    int maxBullet
) :
    m_maxCollectionLimit(maxCollection),
    m_maxBulletLimit(maxBullet),
    m_sunNum(0)
{
    m_productPool.resize(ProductType::MaxProductNum);
    auto pea_factory = std::make_shared<PeaFactory>();
    m_productPool[ProductType::PeaType] = std::make_shared<ObjectPool<ProductObject>>(pea_factory);
    auto sun_factory = std::make_shared<SunFactory>();
    m_productPool[ProductType::SunType] = std::make_shared<ObjectPool<ProductObject>>(sun_factory);
}

int ProductManager::addBullet(ProductType type, int x, int y)
{
    if (type == ProductType::MaxProductNum || m_bulletItems.size() > m_maxBulletLimit) return -1;
    std::shared_ptr<BulletObject> new_bullet = std::static_pointer_cast<BulletObject>(m_productPool[type]->getReusable());
    new_bullet->initilize(x, y);
    m_bulletItems.push_front(new_bullet);
    SDL_Log("ProductManager::addBullet add a bullet\n");
    return 0;
}

int ProductManager::addCollection(ProductType type, int x, int y)
{
    if (type == ProductType::MaxProductNum || m_collectionItems.size() > m_maxCollectionLimit) return -1;
    std::shared_ptr<CollectionObject> new_collection = std::static_pointer_cast<CollectionObject>(m_productPool[type]->getReusable());
    new_collection->initilize(x, y);
    m_collectionItems.push_front(new_collection);
    SDL_Log("ProductManager::addCollection add a collection\n");
    return 0;
}

int ProductManager::produceCollection(ProductType type, int x, int y)
{
    if (type == ProductType::MaxProductNum || m_collectionItems.size() > m_maxCollectionLimit) return -1;
    std::shared_ptr<CollectionObject> new_collection = std::static_pointer_cast<CollectionObject>(m_productPool[type]->getReusable());
    new_collection->initilize(x, y);
    new_collection->setMotion(std::make_shared<MotionAccLimitTime>(15.0f, -120.0f, 0.0f, 200.0f, 1300));
    m_collectionItems.push_front(new_collection);
    SDL_Log("ProductManager::produceCollection produce a collection\n");
    return 0;
}

int ProductManager::randomDropSun()
{
    if (m_collectionItems.size() > m_maxCollectionLimit) return -1;
    float width = Managers::getInstance().mapManager->getRightMargin() - Managers::getInstance().mapManager->getLeftMargin() - 100.0f;
    float gene_x = Managers::getInstance().mapManager->getLeftMargin() + width * rand() / RAND_MAX;
    uint64_t lifetime = 3000 + 10000 * rand() / RAND_MAX;
    std::shared_ptr<CollectionObject> new_collection = std::static_pointer_cast<CollectionObject>(m_productPool[ProductType::SunType]->getReusable());
    new_collection->initilize(gene_x, 0.0f);
    new_collection->setMotion(std::make_shared<MotionSpeedLimitTime>(0.0f, 40.0f, lifetime));
    m_collectionItems.push_front(new_collection);
    SDL_Log("ProductManager::randomDropSun add a sun\n");
    return 0;
}

int ProductManager::calculateDamage(std::shared_ptr<GameObject> other)
{
    int rt_value = 0;
    for (auto iter = m_bulletItems.begin(); iter != m_bulletItems.end(); iter++)
    {
        if (ProductState::Product_MOVE == (*iter)->getState() && (*iter)->collision(other))
        {
            rt_value += (*iter)->getDamage();
            (*iter)->setState(ProductState::Product_DEAD);
        }
    }
    return rt_value;
}

int ProductManager::clickCollection(int mouse_x, int mouse_y)
{
    float real_x = CoreVars::getInstance().camera.getClickX(mouse_x);
    float real_y = CoreVars::getInstance().camera.getClickY(mouse_y);
    for (auto iter = m_collectionItems.begin();iter != m_collectionItems.end(); iter++)
    {
        if ((*iter)->inObject(real_x, real_y))
        {
            switch ((*iter)->getType())
            {
            case ProductType::SunType:
                m_sunNum += 50;
                (*iter)->setMotion(std::make_shared<MotionTargetPoint>(
                    CoreVars::getInstance().camera.getClickX(20.0f),
                    CoreVars::getInstance().camera.getClickY(16.0f),
                    1000.0f)
                );
                break;
            default:
                break;
            }
            (*iter)->setState(ProductState::Product_DEAD);
            return 0;
        }
    }
    return -1;
}

int ProductManager::update()
{
    for (auto iter = m_bulletItems.begin(); iter != m_bulletItems.end();)
    {
        (*iter)->update();
        // 超出屏幕范围子弹检查
        if ((*iter)->m_aabb.x > CoreVars::getInstance().camera.getRight()
            || (*iter)->m_aabb.x + (*iter)->m_aabb.w < CoreVars::getInstance().camera.getLeft()
            || (*iter)->m_aabb.y > CoreVars::getInstance().camera.getBottom()
            || (*iter)->m_aabb.y + (*iter)->m_aabb.h < CoreVars::getInstance().camera.getTop()
            || ProductState::Product_DELETE == (*iter)->getState())
        {
            std::shared_ptr<ProductObject> ptr = std::static_pointer_cast<ProductObject>(*iter);
            iter = m_bulletItems.erase(iter);
            m_productPool[ptr->getType()]->returnReusable(ptr);
            // SDL_Log("remove one\n");
        }
        else
        {
            iter++;
        }
    }
    for (auto iter = m_collectionItems.begin(); iter != m_collectionItems.end();)
    {
        (*iter)->update();
        // 超出屏幕范围子弹检查
        if ((*iter)->m_aabb.x > CoreVars::getInstance().camera.getRight()
            || (*iter)->m_aabb.x + (*iter)->m_aabb.w < CoreVars::getInstance().camera.getLeft()
            || (*iter)->m_aabb.y > CoreVars::getInstance().camera.getBottom()
            || (*iter)->m_aabb.y + (*iter)->m_aabb.h < CoreVars::getInstance().camera.getTop()
            || ProductState::Product_DELETE == (*iter)->getState())
        {
            std::shared_ptr<ProductObject> ptr = std::static_pointer_cast<ProductObject>(*iter);
            iter = m_collectionItems.erase(iter);
            m_productPool[ptr->getType()]->returnReusable(ptr);
            // SDL_Log("remove one\n");
        }
        else
        {
            iter++;
        }
    }
    return 0;
}

int ProductManager::render()
{
    for (auto& ptr : m_bulletItems)
    {
        ptr->render();
    }
    for (auto& ptr : m_collectionItems)
    {
        ptr->render();
    }
    return 0;
}

int ProductManager::clear()
{
    for (auto& ptr : m_bulletItems)
    {
        // 归还
        std::shared_ptr<ProductObject> pptr = ptr;
        m_productPool[ptr->getType()]->returnReusable(pptr);
    }
    for (auto& ptr : m_collectionItems)
    {
        // 归还
        std::shared_ptr<ProductObject> pptr = ptr;
        m_productPool[ptr->getType()]->returnReusable(pptr);
    }
    m_bulletItems.clear();
    m_collectionItems.clear();
    return 0;
}

int ProductManager::collect()
{
    for (auto& ptr : m_collectionItems)
    {
        switch (ptr->getType())
        {
        case ProductType::SunType:
            m_sunNum += 50;
            break;
        default:
            break;
        }
        // 归还
        std::shared_ptr<ProductObject> pptr = ptr;
        m_productPool[ptr->getType()]->returnReusable(pptr);
    }
    m_collectionItems.clear();
    return 0;
}

ProductManager::~ProductManager()
{}