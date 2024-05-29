#include "Scene/LoseScene.h"
#include "Manager/MapManager.h"
#include "Manager/ProductManager.h"
#include "Manager/PlantManager.h"
#include "Manager/ZombieManager.h"
#include "Manager/TaskManager.h"
#include "Manager/CardManager.h"
#include "Core/GlobalVars.h"
#include "Core/AnimLoader.h"

LoseScene::LoseScene() :
    SceneObject(),
    m_animPlayer(
        std::make_shared<AnimLoader>("reanim/ZombiesWon.reanim"),
        SDL_FPoint{ 210.0f, 0.0f }
    )
{
    m_animPlayer.setPlayingTrack({ 0, 1, 2, 3 }, 1);
}

SceneType LoseScene::getType()
{
    return SceneType::Scene_LoseScene;
}


int LoseScene::enterScene()
{
    SDL_Log("enter Lose Scene\n");
    m_stopTimeCountDown = 2000;
    return 0;
}


SceneType LoseScene::handleEvent(SDL_Event& event)
{

    return SceneType::Scene_MaxSceneIdx;
}

SceneType LoseScene::updateScene()
{
    if (!m_animPlayer.isPlayEnd(3))
    {
        m_animPlayer.updatePlayingFrameIdx();
    }
    else
    {
        if (m_stopTimeCountDown <= GlobalVars::getInstance().timer.getDeltaTime())
        {
            return SceneType::Scene_MainScene;
        }
        else
        {
            m_stopTimeCountDown -= GlobalVars::getInstance().timer.getDeltaTime();
        }
    }
    return SceneType::Scene_MaxSceneIdx;
}


int LoseScene::exitScene()
{
    SDL_Log("exit Lose Scene\n");
    return 0;
}


int LoseScene::renderScene()
{
    GlobalVars::getInstance().mapManager->renderMap();
    GlobalVars::getInstance().plantManager->renderPlants();
    GlobalVars::getInstance().zombieManager->renderZombie();
    GlobalVars::getInstance().productManager->render();
    m_animPlayer.render();

    return 0;
}

LoseScene::~LoseScene()
{}