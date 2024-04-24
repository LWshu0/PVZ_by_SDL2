#include "Scene/SelectCardScene.h"
#include "Manager/MapManager.h"
#include "Manager/CardManager.h"

SelectCardScene::SelectCardScene(
    SDL_Renderer* renderer,
    std::shared_ptr<Timer> timer,
    std::shared_ptr<Camera> camera,
    std::shared_ptr<TextureRes> res,
    std::shared_ptr<MapManager> mapManager,
    std::shared_ptr<CardManager> cardManager
) :
    SceneObject(renderer, timer, camera, res),
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
    // 相机位置
    SceneObject::m_camera->setPosition(0.0f, 0.0f);
    m_mapManager->setMap(0.0f, 0.0f, MapType::MapGrassDayOneLine);
    m_cardManager->clearCardSlot();
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
            if (m_cardManager->isFullSlot()) return SceneType::Scene_GameScene;
        }

    }
    return SceneType::Scene_MaxSceneIdx;
}

int SelectCardScene::updateScene()
{
    return 0;
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