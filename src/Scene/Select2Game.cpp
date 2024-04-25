#include "Scene/Select2Game.h"
#include "Manager/MapManager.h"

Select2Game::Select2Game(
    SDL_Renderer* renderer,
    std::shared_ptr<Timer> timer,
    std::shared_ptr<Camera> camera,
    std::shared_ptr<TextureRes> res,
    std::shared_ptr<MapManager> mapManager
) :
    SceneObject(renderer, timer, camera, res),
    m_mapManager(mapManager)
{}

SceneType Select2Game::getType()
{
    return SceneType::Scene_Select2GameScene;
}

int Select2Game::enterScene()
{
    SDL_Log("enter select2game scene\n");
    return 0;
}

SceneType Select2Game::handleEvent(SDL_Event& event)
{
    return SceneType::Scene_MaxSceneIdx;
}

SceneType Select2Game::updateScene()
{
    float delta_x = 0.3f * m_timer->getDeltaTime();
    m_camera->setPosition(m_camera->getX() - delta_x, m_camera->getY());
    if (m_camera->getX() <= 210.0f)
    {
        return SceneType::Scene_GameScene;
    }
    return SceneType::Scene_MaxSceneIdx;
}

int Select2Game::exitScene()
{
    SDL_Log("exit select2game scene\n");
    return 0;
}

int Select2Game::renderScene()
{
    return m_mapManager->renderMap();
}

Select2Game::~Select2Game()
{}
