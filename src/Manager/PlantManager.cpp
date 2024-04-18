#include "Manager/PlantManager.h"

PlantManager::PlantManager(
    SDL_Renderer* renderer,
    std::shared_ptr<TextureRes> res,
    std::shared_ptr<Camera> camera,
    std::shared_ptr<Timer> timer,
    std::shared_ptr<MapManager> mapManager,
    std::shared_ptr<BulletManager> bulletManager
) :
    m_renderer(renderer),
    m_textureRes(res),
    m_camera(camera),
    m_timer(timer),
    m_mapManager(mapManager),
    m_bulletManager(bulletManager)
{
    m_plantTemplate.resize(PlantType::MaxPlantType);
    m_animLoader.resize(PlantType::MaxPlantType);
    // 豌豆射手
    m_animLoader[PlantType::PlantPeaShooter1] = std::make_shared<AnimLoader>("reanim/PeaShooterSingle.reanim", m_renderer, m_textureRes);
    m_animLoader[PlantType::PlantPeaShooter1]->Attach(14, SDL_FPoint{ 32.0f, 63.0f }, 8, SDL_FPoint{ 11.0f, 6.0f });
    m_animLoader[PlantType::PlantPeaShooter1]->Attach(17, SDL_FPoint{ 16.0f, 9.0f }, 14, SDL_FPoint{ 49.0f, 20.0f });
    m_animLoader[PlantType::PlantPeaShooter1]->Attach(16, SDL_FPoint{ 16.0f, 9.0f }, 14, SDL_FPoint{ 49.0f, 20.0f });
    m_plantTemplate[PlantType::PlantPeaShooter1] = std::make_shared<PeaShooterSingle>(m_animLoader[PlantType::PlantPeaShooter1], m_camera, SDL_FPoint{ 0.0f, 0.0f });

}

int PlantManager::initilizePlants()
{
    if (nullptr == m_mapManager) return -1;
    // resize & clear
    m_mainPlants.resize(m_mapManager->getRow());
    for (auto& r : m_mainPlants)
    {
        r.resize(m_mapManager->getCol());
        for (auto& c : r)
        {
            c = nullptr;
        }
    }
    return 0;
}

bool PlantManager::collisionPlant(std::shared_ptr<GameObject> obj, int row, int col)
{
    if (row < 0 || row >= m_mapManager->getRow()
        || col < 0 || col >= m_mapManager->getCol()
        || nullptr == obj
        || nullptr == m_mainPlants[row][col])
    {
        return false;
    }
    return m_mainPlants[row][col]->collision(obj);
}

int PlantManager::addPlant(PlantType type, int row, int col)
{
    if (row < 0 || row >= m_mapManager->getRow()
        || col < 0 || col >= m_mapManager->getCol()
        || PlantType::MaxPlantType == type
        || nullptr != m_mainPlants[row][col])
    {
        return -1;
    }
    float root_x = m_mapManager->getLeftMargin() + col * m_mapManager->getCellWidth();
    float root_y = m_mapManager->getTopMargin() + row * m_mapManager->getCellHeight();
    root_x += m_mapManager->getCellWidth() / 2;
    root_y += m_mapManager->getCellHeight() * 0.8;
    m_mainPlants[row][col] = m_plantTemplate[type]->clonePlant(SDL_FPoint{ root_x, root_y });
    return 0;
}

int PlantManager::removePlant(int row, int col)
{
    if (row < 0 || row >= m_mapManager->getRow()
        || col < 0 || col >= m_mapManager->getCol())
    {
        return -1;
    }
    m_mainPlants[row][col] = nullptr;
    return 0;
}

int PlantManager::doDamage(int row, int col, int d)
{
    if (row < 0 || row >= m_mapManager->getRow()
        || col < 0 || col >= m_mapManager->getCol()
        || nullptr == m_mainPlants[row][col])
    {
        return -1;
    }
    m_mainPlants[row][col]->damage(d);
    return 0;
}

int PlantManager::updatePlants()
{
    for (int i = 0;i < m_mainPlants.size();i++)
    {
        for (int j = 0;j < m_mainPlants[i].size();j++)
        {
            if (nullptr == m_mainPlants[i][j]) continue;

            if (m_mainPlants[i][j]->isDead())
            {
                m_mainPlants[i][j] = nullptr;
            }
            else
            {
                m_mainPlants[i][j]->changePlantState(PlantState::ATTACK, m_timer);
                BulletType bullet_type = m_mainPlants[i][j]->attack(m_timer);
                if (BulletType::MaxBulletType != bullet_type)
                {
                    m_bulletManager->addBullet(bullet_type, m_mainPlants[i][j]->m_aabb.x + m_mainPlants[i][j]->m_aabb.w, m_mainPlants[i][j]->m_aabb.y);
                }

                m_mainPlants[i][j]->updatePlant(m_timer);
            }                                
        }
    }
    return 0;
}

int PlantManager::renderPlants()
{
    for (int i = 0;i < m_mainPlants.size();i++)
    {
        for (int j = 0;j < m_mainPlants[i].size();j++)
        {
            if (nullptr != m_mainPlants[i][j])
            {
                m_mainPlants[i][j]->render();
            }
        }
    }
    return 0;
}

PlantManager::~PlantManager()
{}
