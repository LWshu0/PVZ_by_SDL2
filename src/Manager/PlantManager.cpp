#include "Manager/PlantManager.h"
#include "Manager/MapManager.h"
#include "Manager/BulletManager.h"
#include "Manager/ZombieManager.h"

PlantManager::PlantManager(
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
    m_zombieManager(nullptr)
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

int PlantManager::initilizeManagers(
    std::shared_ptr<MapManager> mapManager,
    std::shared_ptr<BulletManager> bulletManager,
    std::shared_ptr<ZombieManager> zombieManager
)
{
    m_mapManager = mapManager;
    m_bulletManager = bulletManager;
    m_zombieManager = zombieManager;
    return 0;
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
    m_presettlePlantImage = nullptr;
    m_presettleRowIdx = -1;
    m_presettleColIdx = -1;
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

int PlantManager::pickPlant(PlantType type)
{
    if (PlantType::MaxPlantType == type) return -1;
    m_presettleRowIdx = -1;
    m_presettleColIdx = -1;
    m_presettlePlantImage = m_plantTemplate[type]->clonePlant(SDL_FPoint{ 0.0f, 0.0f });
    SDL_Log("plant manager pick a plant: %d\n", type);
    return 0;
}

int PlantManager::presettlePlant(int mouse_x, int mouse_y)
{
    int row = m_mapManager->caculRow(mouse_y);
    int col = m_mapManager->caculCol(mouse_x);
    if (m_mapManager->isValidCell(row, col)
        && m_mainPlants[row][col] == nullptr)
    {
        m_presettleRowIdx = row;
        m_presettleColIdx = col;
        float root_x = m_mapManager->getLeftMargin() + col * m_mapManager->getCellWidth();
        float root_y = m_mapManager->getTopMargin() + row * m_mapManager->getCellHeight();
        root_x += m_mapManager->getCellWidth() / 2;
        root_y += m_mapManager->getCellHeight() * 0.8;
        m_presettlePlantImage->setRootPoint(SDL_FPoint{ root_x,root_y });
    }
    else
    {
        m_presettleRowIdx = -1;
        m_presettleColIdx = -1;
    }
    return 0;
}

int PlantManager::settlePlant()
{
    if (nullptr == m_presettlePlantImage
        || -1 == m_presettleRowIdx
        || -1 == m_presettleColIdx) return -1;
    m_mainPlants[m_presettleRowIdx][m_presettleColIdx] = m_presettlePlantImage;
    m_presettlePlantImage = nullptr;
    m_presettleRowIdx = -1;
    m_presettleColIdx = -1;
    SDL_Log("plant manager settle a plant\n");
    return 0;
}

int PlantManager::putbackPlant()
{
    m_presettlePlantImage = nullptr;
    m_presettleRowIdx = -1;
    m_presettleColIdx = -1;
    SDL_Log("plant manager put back a plant\n");
    return 0;
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
                if (m_zombieManager->hasZombieBetween(i, m_mainPlants[i][j]->m_aabb.x, m_mapManager->getRightMargin()))
                {
                    m_mainPlants[i][j]->changePlantState(PlantState::ATTACK, m_timer);
                }
                else
                {
                    m_mainPlants[i][j]->changePlantState(PlantState::IDLE, m_timer);
                }
                BulletType bullet_type = m_mainPlants[i][j]->attack(m_timer);
                if (BulletType::MaxBulletType != bullet_type)
                {
                    m_bulletManager->addBullet(bullet_type, m_mainPlants[i][j]->m_aabb.x + m_mainPlants[i][j]->m_aabb.w, m_mainPlants[i][j]->m_aabb.y + 10);
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
    if (nullptr != m_presettlePlantImage
        && -1 != m_presettleRowIdx
        && -1 != m_presettleColIdx)
    {
        m_presettlePlantImage->renderStatic(120);
    }
    return 0;
}

void PlantManager::releaseManagers()
{
    m_mapManager = nullptr;
    m_bulletManager = nullptr;
    m_zombieManager = nullptr;
}

PlantManager::~PlantManager()
{}

int PlantManager::changeAllTo(PlantState state)
{
    for (int i = 0;i < m_mainPlants.size();i++)
    {
        for (int j = 0;j < m_mainPlants[i].size();j++)
        {
            if (nullptr != m_mainPlants[i][j])
            {
                m_mainPlants[i][j]->changePlantState(state, m_timer);
            }
        }
    }
    return 0;
}