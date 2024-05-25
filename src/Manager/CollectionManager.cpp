#include "Manager/CollectionManager.h"
#include "Manager/MapManager.h"

CollectionManager::CollectionManager(
    int maxCollection
) :
    m_sunNum(50)
{
    m_collectionTemplate.resize(CollectionType::CollectionMaxNum);

    // 阳光
    std::shared_ptr<AnimLoader> sun_loader = std::make_shared<AnimLoader>("reanim/Sun.reanim");
    m_collectionTemplate[CollectionType::Collection_Sun] = std::make_shared<Sun>(SDL_FRect{ 0.0f,0.0f, 80.0f,80.0f }, sun_loader);
    // ...
    m_collectionItems.resize(maxCollection);
}

int CollectionManager::initilizeManagers(
    std::shared_ptr<MapManager> mapManager
)
{
    m_mapManager = mapManager;
    return 0;
}

int CollectionManager::addCollection(CollectionType type, int x, int y)
{
    if (type == CollectionType::CollectionMaxNum) return -1;
    for (auto& ptr : m_collectionItems)
    {
        if (ptr == nullptr)
        {
            ptr = m_collectionTemplate[type]->cloneCollection(x, y);
            ptr->setMotion(std::make_shared<MotionSpeed>(0.0f, 20.0f));
            SDL_Log("CollectionManager::addCollection add a collection\n");
            return 0;
        }
    }
    return -1;
}

int CollectionManager::randomDropSun()
{
    float width = m_mapManager->getRightMargin() - m_mapManager->getLeftMargin() - 100.0f;
    float gene_x = m_mapManager->getLeftMargin() + width * rand() / RAND_MAX;
    uint64_t lifetime = 3000 + 10000 * rand() / RAND_MAX;
    for (auto& ptr : m_collectionItems)
    {
        if (ptr == nullptr)
        {
            ptr = m_collectionTemplate[CollectionType::Collection_Sun]->cloneCollection(gene_x, 0.0f);
            ptr->setMotion(std::make_shared<MotionSpeedLimitTime>(0.0f, 40.0f, lifetime));
            SDL_Log("CollectionManager::randomDropSun add a sun\n");
            return 0;
        }
    }
    return -1;
}

int CollectionManager::clickCollection(int mouse_x, int mouse_y)
{
    float real_x = GlobalVars::getInstance().camera.getClickX(mouse_x);
    float real_y = GlobalVars::getInstance().camera.getClickY(mouse_y);
    for (auto& ptr : m_collectionItems)
    {
        if (ptr != nullptr && ptr->inObject(real_x, real_y))
        {
            switch (ptr->getType())
            {
            case CollectionType::Collection_Sun:
                m_sunNum += 50;
                break;
            default:
                break;
            }
            ptr = nullptr;
            return 0;
        }
    }
    return -1;
}

int CollectionManager::updateCollection()
{
    for (auto& ptr : m_collectionItems)
    {
        if (ptr != nullptr)
        {
            ptr->updateCollection();
        }
    }
    return 0;
}

int CollectionManager::renderCollection()
{
    for (auto& ptr : m_collectionItems)
    {
        if (ptr != nullptr)
        {
            ptr->render();
        }
    }
    return 0;
}

int CollectionManager::clearCollection()
{
    for (auto& ptr : m_collectionItems)
    {
        ptr = nullptr;
    }
    return 0;
}

int CollectionManager::collectCollection()
{
    for (auto& ptr : m_collectionItems)
    {
        if (ptr != nullptr)
        {
            switch (ptr->getType())
            {
            case CollectionType::Collection_Sun:
                m_sunNum += 50;
                break;
            default:
                break;
            }
            ptr = nullptr;
        }
    }
    return 0;
}

void CollectionManager::releaseManagers()
{
    m_mapManager = nullptr;
}

CollectionManager::~CollectionManager()
{}