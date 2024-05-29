#include "Scene/SelectCardScene.h"
#include "Manager/MapManager.h"
#include "Manager/CardManager.h"
#include "Manager/UsrInfoManager.h"
#include "Core/GlobalVars.h"

SelectCardScene::SelectCardScene() :
    SceneObject(),
    m_startButtonRange(SDL_Rect{ 144, 550, 156, 42 }),
    m_enableFont("data/_BrianneTod16.png", "data/BrianneTod16.txt", SDL_Color{ 255, 255, 0, 255 }),
    m_disableFont("data/_BrianneTod16.png", "data/BrianneTod16.txt", SDL_Color{ 205, 64, 40, 255 })
{
    m_startButtonBK = GlobalVars::getInstance().textureRes.getTextureFrom("images/SeedChooser_Button.png");
    m_startButtonDisable = GlobalVars::getInstance().textureRes.getTextureFrom("images/SeedChooser_Button_Disabled.png");
    m_startButtonBloom = GlobalVars::getInstance().textureRes.getTextureFrom("images/SeedChooser_Button_Glow.png");
}

SceneType SelectCardScene::getType()
{
    return SceneType::Scene_SelectCardScene;
}

int SelectCardScene::enterScene()
{
    SDL_Log("enter select card scene\n");
    GlobalVars::getInstance().mapManager->setMap(MapType::MapGrassDayOneLine);
    GlobalVars::getInstance().cardManager->clearCardSlot();
    GlobalVars::getInstance().cardManager->resetCardPool();
    // 相机位置 在地图初始化之后
    GlobalVars::getInstance().camera.setPosition(GlobalVars::getInstance().mapManager->getWidth() - 800, 0.0f);
    m_enableStartButton = false;
    m_hoverStartButton = false;
    return 0;
}

SceneType SelectCardScene::handleEvent(SDL_Event& event)
{
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q) return SceneType::Scene_MainScene;
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) // 左键 尝试选卡
    {
        // 选卡过程
        SDL_Log("mouse button left\n");
        int slot_idx = GlobalVars::getInstance().cardManager->getSlotIdx(event.button.x, event.button.y);
        if (slot_idx != -1)
        {
            SDL_Log("slot -> pool: %d\n", slot_idx);
            GlobalVars::getInstance().cardManager->slot2pool(slot_idx);
            m_enableStartButton = GlobalVars::getInstance().cardManager->isFullSlot();
            return SceneType::Scene_MaxSceneIdx;
        }
        int pool_idx = GlobalVars::getInstance().cardManager->getPoolIdx(event.button.x, event.button.y);
        if (GlobalVars::getInstance().usrinfoManager->isUnlockPlant(pool_idx))
        {
            SDL_Log("pool -> slot: %d\n", pool_idx);
            GlobalVars::getInstance().cardManager->pool2slot(pool_idx);
            m_enableStartButton = GlobalVars::getInstance().cardManager->isFullSlot();
            return SceneType::Scene_MaxSceneIdx;
        }
        // 点击按钮开始游戏
        if (m_startButtonRange.isInShape(event.button.x, event.button.y) && m_enableStartButton)
        {
            return SceneType::Scene_Select2GameScene;
        }
    }
    else if (event.type == SDL_MOUSEMOTION)
    {
        m_hoverStartButton = m_startButtonRange.isInShape(event.button.x, event.button.y);
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
    // 绘制按钮
    if (m_enableStartButton)
    {
        SDL_RenderCopy(GlobalVars::getInstance().renderer, m_startButtonBK, NULL, &m_startButtonRange.m_range);
        m_enableFont.render("一起开始吧!", m_startButtonRange.m_range.x, m_startButtonRange.m_range.y, 1.5f);
        if (m_hoverStartButton) SDL_RenderCopy(GlobalVars::getInstance().renderer, m_startButtonBloom, NULL, &m_startButtonRange.m_range);
    }
    else
    {
        SDL_RenderCopy(GlobalVars::getInstance().renderer, m_startButtonDisable, NULL, &m_startButtonRange.m_range);
        m_disableFont.render("一起开始吧!", m_startButtonRange.m_range.x, m_startButtonRange.m_range.y, 1.5f);
    }
    return 0;
}

SelectCardScene::~SelectCardScene()
{}