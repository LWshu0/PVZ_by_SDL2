#include "Manager/ProductManager.h"
#include "Manager/MapManager.h"

ProductManager::ProductManager(
    int maxCollection,
    int maxBullet
) :
    m_maxCollectionLimit(maxCollection),
    m_maxBulletLimit(maxBullet),
    m_sunNum(0)
{
    m_productTemplate.resize(ProductType::MaxProductNum);
    m_productTemplate[ProductType::PeaType] = std::make_shared<Pea>(0.0f, 0.0f);
    std::shared_ptr<AnimLoader> sun_loader = std::make_shared<AnimLoader>("reanim/Sun.reanim");
    m_productTemplate[ProductType::SunType] = std::make_shared<Sun>(SDL_FRect{ 0.0f,0.0f, 80.0f,80.0f }, sun_loader);
}

int ProductManager::initilizeManagers(
    std::shared_ptr<MapManager> mapManager
)
{
    m_mapManager = mapManager;
    return 0;
}

int ProductManager::addBullet(ProductType type, int x, int y)
{
    if (type == ProductType::MaxProductNum || m_bulletItems.size() > m_maxBulletLimit) return -1;
    m_bulletItems.push_front(std::static_pointer_cast<BulletObject>(m_productTemplate[type]->clone(x, y)));
    SDL_Log("ProductManager::addBullet add a bullet\n");
    return 0;
}

int ProductManager::addCollection(ProductType type, int x, int y)
{
    if (type == ProductType::MaxProductNum || m_collectionItems.size() > m_maxCollectionLimit) return -1;
    m_collectionItems.push_front(std::static_pointer_cast<CollectionObject>(m_productTemplate[type]->clone(x, y)));
    SDL_Log("ProductManager::addCollection add a collection\n");
    return 0;
}

int ProductManager::produceCollection(ProductType type, int x, int y)
{
    if (type == ProductType::MaxProductNum || m_collectionItems.size() > m_maxCollectionLimit) return -1;
    m_collectionItems.push_front(std::static_pointer_cast<CollectionObject>(m_productTemplate[type]->clone(x, y)));
    m_collectionItems.front()->setMotion(std::make_shared<MotionAccLimitTime>(20.0f, -20.0f, 0.0f, 10.0f, 2000));
    SDL_Log("ProductManager::produceCollection produce a collection\n");
    return 0;
}

int ProductManager::randomDropSun()
{
    if (m_collectionItems.size() > m_maxCollectionLimit) return -1;
    float width = m_mapManager->getRightMargin() - m_mapManager->getLeftMargin() - 100.0f;
    float gene_x = m_mapManager->getLeftMargin() + width * rand() / RAND_MAX;
    uint64_t lifetime = 3000 + 10000 * rand() / RAND_MAX;
    m_collectionItems.push_front(std::static_pointer_cast<CollectionObject>(m_productTemplate[ProductType::SunType]->clone(gene_x, 0.0f)));
    m_collectionItems.front()->setMotion(std::make_shared<MotionSpeedLimitTime>(0.0f, 40.0f, lifetime));
    SDL_Log("ProductManager::randomDropSun add a sun\n");
    return 0;
}

int ProductManager::calculateDamage(std::shared_ptr<GameObject> other)
{
    int rt_value = 0;
    for (auto iter = m_bulletItems.begin(); iter != m_bulletItems.end();)
    {
        if ((*iter)->collision(other))
        {
            rt_value += (*iter)->getDamage();
            iter = m_bulletItems.erase(iter);
        }
        else
        {
            iter++;
        }
    }
    return rt_value;
}

int ProductManager::clickCollection(int mouse_x, int mouse_y)
{
    float real_x = GlobalVars::getInstance().camera.getClickX(mouse_x);
    float real_y = GlobalVars::getInstance().camera.getClickY(mouse_y);
    for (auto iter = m_collectionItems.begin();iter != m_collectionItems.end(); iter++)
    {
        if ((*iter)->inObject(real_x, real_y))
        {
            switch ((*iter)->getType())
            {
            case ProductType::SunType:
                m_sunNum += 50;
                break;
            default:
                break;
            }
            m_collectionItems.erase(iter);
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
        if ((*iter)->m_aabb.x > GlobalVars::getInstance().camera.getRight()
            || (*iter)->m_aabb.x + (*iter)->m_aabb.w < GlobalVars::getInstance().camera.getLeft()
            || (*iter)->m_aabb.y > GlobalVars::getInstance().camera.getBottom()
            || (*iter)->m_aabb.y + (*iter)->m_aabb.h < GlobalVars::getInstance().camera.getTop())
        {
            iter = m_bulletItems.erase(iter);
            // SDL_Log("remove one\n");
        }
        else
        {
            iter++;
        }
    }
    for (auto& ptr : m_collectionItems)
    {
        ptr->update();
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
    }
    m_collectionItems.clear();
    return 0;
}

void ProductManager::releaseManagers()
{
    m_mapManager = nullptr;
}

ProductManager::~ProductManager()
{}