#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "SceneObject.h"
#include "Plants/PlantObject.h"

class MapManager;
class BulletManager;
class PlantManager;
class ZombieManager;
class TaskManager;
class CardManager;

class GameScene : public SceneObject {
protected:
    // 游戏进程
    int m_cardInHandIdx;                    // 正在手中的卡槽编号
    PlantType m_plantInHandType;            // 正在手中的植物类型
    int m_mousePositionX, m_mousePositionY; // 鼠标移动的位置
    // 游戏对象 manager
    std::shared_ptr<MapManager> m_mapManager;
    std::shared_ptr<BulletManager> m_bulletManager;
    std::shared_ptr<PlantManager> m_plantManager;
    std::shared_ptr<ZombieManager> m_zombieManager;
    std::shared_ptr<TaskManager> m_taskManager;
    std::shared_ptr<CardManager> m_cardManager;
public:
    GameScene(
        SDL_Renderer* renderer,
        std::shared_ptr<Timer> timer,
        std::shared_ptr<Camera> camera,
        std::shared_ptr<TextureRes> res,
        std::shared_ptr<MapManager> mapManager,
        std::shared_ptr<BulletManager> bulletManager,
        std::shared_ptr<PlantManager> plantManager,
        std::shared_ptr<ZombieManager> zombieManager,
        std::shared_ptr<TaskManager> taskManager,
        std::shared_ptr<CardManager> cardManager
        );

    virtual SceneType getType() override;

    virtual int enterScene() override;

    virtual SceneType handleEvent(SDL_Event& event) override;

    virtual SceneType updateScene() override;

    virtual int exitScene() override;

    virtual int renderScene() override;

    ~GameScene();
};




#endif