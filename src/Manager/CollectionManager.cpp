#include "Manager/CollectionManager.h"

CollectionManager::CollectionManager(
    SDL_Renderer* renderer,
    std::shared_ptr<TextureRes> texture_res,
    std::shared_ptr<Camera> camera,
    std::shared_ptr<Timer> timer,
    int maxCollection
) :
    m_renderer(renderer),
    m_textureRes(texture_res),
    m_camera(camera),
    m_timer(timer)
{
    m_collectionTemplate.resize(CollectionType::CollectionMaxNum);
    // 定义运动方式
    m_dropMotion = std::make_shared<MotionSpeed>(0.0f, 20.0f);
    // 阳光
    std::shared_ptr<AnimLoader> sun_loader = std::make_shared<AnimLoader>("reanim/Sun.reanim", renderer, texture_res);
    m_collectionTemplate[CollectionType::Collection_Sun] = std::make_shared<Sun>(SDL_FRect{ 0.0f,0.0f, 80.0f,80.0f }, sun_loader, camera);
    // ...
    m_collectionItems.resize(maxCollection);
}

int CollectionManager::addCollection(CollectionType type, int x, int y)
{
    if (type == CollectionType::CollectionMaxNum) return -1;
    for (auto& ptr : m_collectionItems)
    {
        if (ptr == nullptr)
        {
            ptr = m_collectionTemplate[type]->cloneCollection(x, y);
            ptr->setMotion(m_dropMotion);
            SDL_Log("CollectionManager::addCollection add a collection\n");
            return 0;
        }
    }
    return -1;
}

int CollectionManager::clickCollection(int mouse_x, int mouse_y)
{
    float real_x = m_camera->getClickX(mouse_x);
    float real_y = m_camera->getClickY(mouse_y);
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
            ptr->updateCollection(m_timer);
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
            ptr->renderCollection();
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

CollectionManager::~CollectionManager()
{}