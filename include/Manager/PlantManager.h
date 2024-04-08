#ifndef PLANTMANAGER_H
#define PLANTMANAGER_H

#include "MapManager.h"
#include "Plants/PlantObject.h"
#include <vector>
#include <memory>

class PlantManager {
protected:
    // 关联 manager
    std::shared_ptr<MapManager> m_mapManager;
    // plants in game
    std::vector<std::vector<std::shared_ptr<PlantObject>>> m_mainPlants;

public:
    PlantManager(std::shared_ptr<MapManager> mapManager);

    int initilizePlants();

    int addPlant(int row, int col, std::shared_ptr<PlantObject> plant);
    int removePlant(int row, int col);
    int doDamage(int row, int col, int d);

    int renderPlants(uint64_t now_ms);

    ~PlantManager();
};

#endif