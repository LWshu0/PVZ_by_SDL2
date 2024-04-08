#include "Manager/PlantManager.h"
PlantManager::PlantManager(std::shared_ptr<MapManager> mapManager) :
    m_mapManager(mapManager)
{}

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

int PlantManager::addPlant(int row, int col, std::shared_ptr<PlantObject> plant)
{
    if (row < 0 || row >= m_mapManager->getRow()
        || col < 0 || col >= m_mapManager->getCol()
        || nullptr == plant
        || nullptr != m_mainPlants[row][col])
    {
        return -1;
    }
    float root_x = m_mapManager->getLeftMargin() + col * m_mapManager->getCellWidth();
    float root_y = m_mapManager->getTopMargin() + row * m_mapManager->getCellHeight();
    root_x += m_mapManager->getCellWidth() / 2;
    root_y += m_mapManager->getCellHeight() * 0.8;
    m_mainPlants[row][col] = plant->createPlant(SDL_FPoint{ root_x, root_y });
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

int PlantManager::renderPlants(uint64_t now_ms)
{
    for (int i = 0;i < m_mainPlants.size();i++)
    {
        for (int j = 0;j < m_mainPlants[i].size();j++)
        {
            if (nullptr != m_mainPlants[i][j]) m_mainPlants[i][j]->Play(now_ms);
        }
    }
    // for (auto& r : m_mainPlants)
    // {
    //     for (auto& c : r)
    //     {
    //         if(nullptr != c) c->Play(now_ms);
    //     }
    // }
    return 0;
}

PlantManager::~PlantManager()
{}
