#include "Manager/SceneManager.h"

SceneManager::SceneManager(
    SDL_Renderer* renderer,
    std::shared_ptr<TextureRes> res,
    std::shared_ptr<Camera> camera,
    std::shared_ptr<Timer> timer,
    std::shared_ptr<MapManager> mapManager,
    std::shared_ptr<BulletManager> bulletManager,
    std::shared_ptr<PlantManager> plantManager,
    std::shared_ptr<ZombieManager> zombieManager,
    std::shared_ptr<TaskManager> taskManager,
    std::shared_ptr<CardManager> cardManager
) :
    m_renderer(renderer),
    m_textureRes(res),
    m_camera(camera),
    m_timer(timer)
{
    // 分配空间
    m_sceneTemplate.resize(SceneType::Scene_MaxSceneIdx);
    // 主界面
    std::shared_ptr<AnimLoader> main_scene_loader = std::make_shared<AnimLoader>("reanim/SelectorScreen.reanim", renderer, res);
    m_sceneTemplate[SceneType::Scene_MainScene] = std::make_shared<MainScene>(renderer, timer, camera, res, main_scene_loader);
    // 游戏界面
    m_sceneTemplate[SceneType::Scene_GameScene] = std::make_shared<GameScene>(renderer, timer, camera, res, mapManager, bulletManager, plantManager, zombieManager, taskManager, cardManager);
    // ...

    // 设置初始界面
    m_currentScene = m_sceneTemplate[SceneType::Scene_MainScene];
    m_currentScene->enterScene();
}

int SceneManager::handleEvent(SDL_Event& event)
{
    SceneType next_scene = m_currentScene->handleEvent(event);
    if (next_scene != SceneType::Scene_MaxSceneIdx)
    {
        m_currentScene->exitScene();
        m_currentScene = m_sceneTemplate[next_scene];
        m_currentScene->enterScene();
    }
    return 0;
}

int SceneManager::updateScene()
{
    return m_currentScene->updateScene();
}

int SceneManager::renderScene()
{
    return m_currentScene->renderScene();
}

SceneManager::~SceneManager()
{

}