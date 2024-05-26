#include "Scene/Select2Game.h"
#include "Manager/MapManager.h"

Select2Game::Select2Game() :
    SceneObject()
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
    float delta_x = 0.3f * GlobalVars::getInstance().timer.getDeltaTime();
    GlobalVars::getInstance().camera.setPosition(GlobalVars::getInstance().camera.getX() - delta_x, GlobalVars::getInstance().camera.getY());
    if (GlobalVars::getInstance().camera.getX() <= 210.0f)
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
    return GlobalVars::getInstance().mapManager->renderMap();
}

Select2Game::~Select2Game()
{}
