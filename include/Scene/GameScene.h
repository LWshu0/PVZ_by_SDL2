#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "SceneObject.h"

class MapManager;
class BulletManager;
class PlantManager;
class ZombieManager;

class GameScene : public SceneObject {
protected:
    // 游戏对象 manager
    std::shared_ptr<MapManager> m_mapManager;
    std::shared_ptr<BulletManager> m_bulletManager;
    std::shared_ptr<PlantManager> m_plantManager;
    std::shared_ptr<ZombieManager> m_zombieManager;

public:
    GameScene(
        SDL_Renderer* renderer,
        std::shared_ptr<Timer> timer,
        std::shared_ptr<Camera> camera,
        std::shared_ptr<TextureRes> res,
        std::shared_ptr<MapManager> mapManager,
        std::shared_ptr<BulletManager> bulletManager,
        std::shared_ptr<PlantManager> plantManager,
        std::shared_ptr<ZombieManager> zombieManager
    );

    virtual SceneType getType() override;

    virtual int enterScene() override;

    virtual SceneType handleEvent(SDL_Event& event) override;

    virtual int updateScene() override;

    virtual int exitScene() override;

    virtual int renderScene() override;

    ~GameScene();
};




#endif