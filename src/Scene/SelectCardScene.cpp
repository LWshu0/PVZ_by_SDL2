#include "Scene/SelectCardScene.h"
#include "Manager/MapManager.h"
#include "Manager/CardManager.h"

SelectCardScene::SelectCardScene(
    std::shared_ptr<MapManager> mapManager,
    std::shared_ptr<CardManager> cardManager
) :
    SceneObject(),
    m_mapManager(mapManager),
    m_cardManager(cardManager)
{}

SceneType SelectCardScene::getType()
{
    return SceneType::Scene_SelectCardScene;
}


int SelectCardScene::enterScene()
{
    SDL_Log("enter select card scene\n");
    m_mapManager->setMap(0.0f, 0.0f, MapType::MapGrassDayOneLine);
    m_cardManager->clearCardSlot();
    m_cardManager->resetCardPool();
    // 相机位置 在地图初始化之后
    GlobalVars::getInstance().camera.setPosition(m_mapManager->getWidth() - 800, 0.0f);
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
            int slot_idx = m_cardManager->getSlotIdx(event.button.x, event.button.y);
            if (slot_idx != -1)
            {
                SDL_Log("slot -> pool: %d\n", slot_idx);
                m_cardManager->slot2pool(slot_idx);
                return SceneType::Scene_MaxSceneIdx;
            }
            int pool_idx = m_cardManager->getPoolIdx(event.button.x, event.button.y);
            if (pool_idx != -1)
            {
                SDL_Log("pool -> slot: %d\n", pool_idx);
                m_cardManager->pool2slot(pool_idx);
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
    m_mapManager->renderMap();
    // m_zombieManager->renderZombie();
    m_cardManager->renderCardSlot();
    m_cardManager->renderCardPool();
    return 0;
}

SelectCardScene::~SelectCardScene()
{}