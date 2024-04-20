#include "Scene/GameScene.h"
#include "Manager/MapManager.h"
#include "Manager/BulletManager.h"
#include "Manager/PlantManager.h"
#include "Manager/ZombieManager.h"

GameScene::GameScene(
    SDL_Renderer* renderer,
    std::shared_ptr<Timer> timer,
    std::shared_ptr<Camera> camera,
    std::shared_ptr<TextureRes> res,
    std::shared_ptr<MapManager> mapManager,
    std::shared_ptr<BulletManager> bulletManager,
    std::shared_ptr<PlantManager> plantManager,
    std::shared_ptr<ZombieManager> zombieManager
) :
    SceneObject(renderer, timer, camera, res),
    m_mapManager(mapManager),
    m_bulletManager(bulletManager),
    m_plantManager(plantManager),
    m_zombieManager(zombieManager)
{

}

SceneType GameScene::getType()
{
    return SceneType::Scene_GameScene;
}

int GameScene::enterScene()
{
    SDL_Log("enter game scene\n");
    return 0;
}

SceneType GameScene::handleEvent(SDL_Event& event)
{
    if (event.type == SDL_MOUSEBUTTONDOWN) return SceneType::Scene_MainScene;

    return SceneType::Scene_MaxSceneIdx;
}

int GameScene::updateScene()
{
    return 0;
}

int GameScene::exitScene()
{
    SDL_Log("exit game scene\n");
    return 0;
}

int GameScene::renderScene()
{
    return 0;
}

GameScene::~GameScene()
{}