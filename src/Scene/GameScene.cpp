#include "Scene/GameScene.h"
#include "Manager/MapManager.h"
#include "Manager/BulletManager.h"
#include "Manager/PlantManager.h"
#include "Manager/ZombieManager.h"
#include "Manager/TaskManager.h"
#include "Manager/CardManager.h"

GameScene::GameScene(
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
) :
    SceneObject(renderer, timer, camera, res),
    m_mapManager(mapManager),
    m_bulletManager(bulletManager),
    m_plantManager(plantManager),
    m_zombieManager(zombieManager),
    m_taskManager(taskManager),
    m_cardManager(cardManager)
{

}

SceneType GameScene::getType()
{
    return SceneType::Scene_GameScene;
}

int GameScene::enterScene()
{
    SDL_Log("enter game scene\n");
    m_mapManager->setMap(0.0f, 0.0f, MapType::MapGrassDayOneLine);
    m_plantManager->initilizePlants();
    m_zombieManager->initilizeZombie();
    m_bulletManager->clearBullets();
    m_taskManager->loadTask("task/1-1-1.xml");
    if (0 == m_plantManager->addPlant(PlantType::PlantPeaShooter1, 0, 0)) { SDL_Log("add plant at (0, 0)\n"); }
    if (0 == m_plantManager->addPlant(PlantType::PlantPeaShooter1, 0, 1)) { SDL_Log("add plant at (0, 1)\n"); }
    if (0 == m_plantManager->addPlant(PlantType::PlantPeaShooter1, 1, 1)) { SDL_Log("add plant at (1, 1)\n"); }
    // if (0 == plant_manager->removePlant(1, 1)) { SDL_Log("remove plant at (1, 1)\n"); }

    // if (0 == m_zombieManager->addZombie(ZombieType::ZombieNormal, 0, 5)) { SDL_Log("add zombie at (0, 0)\n"); }
    return 0;
}

SceneType GameScene::handleEvent(SDL_Event& event)
{
    if (event.type == SDL_MOUSEBUTTONDOWN) return SceneType::Scene_MainScene;

    return SceneType::Scene_MaxSceneIdx;
}

int GameScene::updateScene()
{
    m_taskManager->updateTask();
    m_plantManager->updatePlants();
    m_bulletManager->updateBullets();
    m_zombieManager->updateZombie();
    m_zombieManager->attackPlants();
    return 0;
}

int GameScene::exitScene()
{
    SDL_Log("exit game scene\n");
    return 0;
}

int GameScene::renderScene()
{
    m_mapManager->renderMap();
    m_plantManager->renderPlants();
    m_zombieManager->renderZombie();
    m_bulletManager->renderBullets();
    m_cardManager->renderCard();
    return 0;
}

GameScene::~GameScene()
{}