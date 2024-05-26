#include "Manager/SceneManager.h"

SceneManager::SceneManager()
{
    // 分配空间
    m_sceneTemplate.resize(SceneType::Scene_MaxSceneIdx);
    // 主界面
    m_sceneTemplate[SceneType::Scene_MainScene] = std::make_shared<MainScene>();
    // 选卡界面
    m_sceneTemplate[SceneType::Scene_SelectCardScene] = std::make_shared<SelectCardScene>();
    // 选卡到游戏的过渡界面
    m_sceneTemplate[SceneType::Scene_Select2GameScene] = std::make_shared<Select2Game>();
    // 游戏界面
    m_sceneTemplate[SceneType::Scene_GameScene] = std::make_shared<GameScene>();
    // 获胜界面
    m_sceneTemplate[SceneType::Scene_WinScene] = std::make_shared<WinScene>();
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

    SceneType next_scene = m_currentScene->updateScene();
    if (next_scene != SceneType::Scene_MaxSceneIdx)
    {
        m_currentScene->exitScene();
        m_currentScene = m_sceneTemplate[next_scene];
        m_currentScene->enterScene();
    }
    return 0;
}

int SceneManager::renderScene()
{
    return m_currentScene->renderScene();
}

SceneManager::~SceneManager()
{

}