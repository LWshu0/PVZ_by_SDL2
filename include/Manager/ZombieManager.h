#ifndef ZOMBIEMANAGER_H
#define ZOMBIEMANAGER_H

#include <vector>
#include <memory>
#include <mutex>

#include "Core/TextureRes.h"
#include "Core/AnimLoader.h"
#include "Core/Camera.h"
#include "Core/Timer.h"
#include "Manager/MapManager.h"
#include "Manager/BulletManager.h"
#include "Manager/PlantManager.h"

#include "Zombies/ZombieObject.h"
#include "Zombies/Zombie.h"

enum ZombieType {
    ZombieNormal,
    MaxZombieType
};

class ZombieManager {
protected:
    /**************************
    *    初始化后不可变成员   *
    **************************/
    SDL_Renderer* m_renderer;                                   // 渲染器
    std::shared_ptr<TextureRes> m_textureRes;                   // 纹理资源
    std::shared_ptr<Camera> m_camera;                           // 相机
    std::shared_ptr<Timer> m_timer;                             // 游戏时钟
    std::vector<std::shared_ptr<AnimLoader>> m_animLoader;      // 植物动画资源
    std::vector<std::shared_ptr<ZombieObject>> m_zombieTemplate;  // 植物模板

    // 关联 manager
    std::shared_ptr<MapManager> m_mapManager;
    std::shared_ptr<BulletManager> m_bulletManager;
    std::shared_ptr<PlantManager> m_plantManager;

    // zombies in game
    std::vector<std::shared_ptr<ZombieObject>> m_zombies;

    std::mutex m_mutex;
public:
    ZombieManager(
        SDL_Renderer* renderer,
        std::shared_ptr<TextureRes> res,
        std::shared_ptr<Camera> camera,
        std::shared_ptr<Timer> timer,
        std::shared_ptr<MapManager> mapManager,
        std::shared_ptr<BulletManager> bulletManager,
        std::shared_ptr<PlantManager> plantManager
    );

    int initilizeZombie();

    int addZombie(ZombieType type, int row, int col);

    // 碰撞检测
    int updateZombie();
    // 攻击检测
    int attackPlants();

    int renderZombie();

    ~ZombieManager();

    // 测试
    int changeAllTo(ZombieState state);
};

#endif