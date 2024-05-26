#include "Scene/SelectCardScene.h"
#include "Manager/MapManager.h"
#include "Manager/CardManager.h"

SelectCardScene::SelectCardScene() :
    SceneObject()
{}

SceneType SelectCardScene::getType()
{
    return SceneType::Scene_SelectCardScene;
}


int SelectCardScene::enterScene()
{
    SDL_Log("enter select card scene\n");
    GlobalVars::getInstance().mapManager->setMap(0.0f, 0.0f, MapType::MapGrassDayOneLine);
    GlobalVars::getInstance().cardManager->clearCardSlot();
    GlobalVars::getInstance().cardManager->resetCardPool();
    // 相机位置 在地图初始化之后
    GlobalVars::getInstance().camera.setPosition(GlobalVars::getInstance().mapManager->getWidth() - 800, 0.0f);
    return 0;
}

SceneType SelectCardScene::handleEvent(SDL_Event& event)
{
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q) return SceneType::Scene_MainScene;
    if (event.type == SDL_MOUSEBUTTONDOWN)
    {
        // 选卡过程
        if (event.button.button == SDL_BUTTON_LEFT) // 左键 尝试选卡 or 收集阳光/银币
        {
            SDL_Log("mouse button left\n");
            int slot_idx = GlobalVars::getInstance().cardManager->getSlotIdx(event.button.x, event.button.y);
            if (slot_idx != -1)
            {
                SDL_Log("slot -> pool: %d\n", slot_idx);
                GlobalVars::getInstance().cardManager->slot2pool(slot_idx);
                return SceneType::Scene_MaxSceneIdx;
            }
            int pool_idx = GlobalVars::getInstance().cardManager->getPoolIdx(event.button.x, event.button.y);
            if (pool_idx != -1)
            {
                SDL_Log("pool -> slot: %d\n", pool_idx);
                GlobalVars::getInstance().cardManager->pool2slot(pool_idx);
                return SceneType::Scene_MaxSceneIdx;
            }
            // 点击按钮开始游戏
            // todo ...
        }
        else
        {
            // 临时: 非左键且满足条件就开始游戏
            // if (m_cardManager->isFullSlot())
                return SceneType::Scene_Select2GameScene;
        }

    }
    return SceneType::Scene_MaxSceneIdx;
}

SceneType SelectCardScene::updateScene()
{
    return SceneType::Scene_MaxSceneIdx;
}

int SelectCardScene::exitScene()
{
    SDL_Log("exit select card scene\n");
    return 0;
}

int SelectCardScene::renderScene()
{
    GlobalVars::getInstance().mapManager->renderMap();
    // m_zombieManager->renderZombie();
    GlobalVars::getInstance().cardManager->renderCardSlot();
    GlobalVars::getInstance().cardManager->renderCardPool();
    return 0;
}

SelectCardScene::~SelectCardScene()
{}