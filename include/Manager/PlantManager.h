#ifndef PLANTMANAGER_H
#define PLANTMANAGER_H

#include <vector>
#include <memory>

#include "MapManager.h"
#include "BulletManager.h"
#include "Plants/PlantObject.h"
#include "Plants/PeaShooterSingle.h"

enum PlantType {
    PlantPeaShooter1,
    MaxPlantType
};

class PlantManager {
protected:
    /**************************
    *    初始化后不可变成员   *
    **************************/
    SDL_Renderer* m_renderer;                                   // 渲染器
    std::shared_ptr<TextureRes> m_textureRes;                   // 纹理资源
    std::shared_ptr<Camera> m_camera;                           // 相机
    std::shared_ptr<Timer> m_timer;                             // 游戏时钟
    std::vector<std::shared_ptr<AnimLoader>> m_animLoader;      // 植物动画资源
    std::vector<std::shared_ptr<PlantObject>> m_plantTemplate;  // 植物模板

    // 关联 manager
    std::shared_ptr<MapManager> m_mapManager;
    std::shared_ptr<BulletManager> m_bulletManager;
    // plants in game
    std::vector<std::vector<std::shared_ptr<PlantObject>>> m_mainPlants;

public:
    PlantManager(
        SDL_Renderer* renderer,
        std::shared_ptr<TextureRes> res,
        std::shared_ptr<Camera> camera,
        std::shared_ptr<Timer> timer,
        std::shared_ptr<MapManager> mapManager,
        std::shared_ptr<BulletManager> bulletManager
        );

    int initilizePlants();

    int addPlant(PlantType type, int row, int col);
    int removePlant(int row, int col);
    int doDamage(int row, int col, int d);

    int updatePlants();
    int renderPlants();

    ~PlantManager();
};

#endif