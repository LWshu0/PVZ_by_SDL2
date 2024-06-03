#include "Scene/GameScene.h"
#include "Manager/MapManager.h"
#include "Manager/ProductManager.h"
#include "Manager/PlantManager.h"
#include "Manager/ZombieManager.h"
#include "Manager/TaskManager.h"
#include "Manager/CardManager.h"
// 全局单例
#include "Core/CoreVars.h"
#include "Resource/ResVars.h"

GameScene::GameScene() :
    SceneObject(),
    m_cardInHandIdx(-1),
    m_plantInHandType(PlantType::MaxPlantType),
    m_dropSunIntervalTime(5000),
    m_dropSunCountDown(5000)
{

}

SceneType GameScene::getType()
{
    return SceneType::Scene_GameScene;
}

int GameScene::enterScene()
{
    SDL_Log("enter game scene\n");
    m_cardInHandIdx = -1;
    m_plantInHandType = PlantType::MaxPlantType;
    m_dropSunCountDown = 5000;
    // Managers::getInstance().mapManager->setMap(MapType::MapGrassDayOneLine);
    Managers::getInstance().plantManager->initilizePlants();
    Managers::getInstance().zombieManager->initilizeZombie();
    Managers::getInstance().productManager->clear();
    Managers::getInstance().taskManager->loadTask();
    Managers::getInstance().productManager->setSunNum(50);
    // if (0 == m_plantManager->addPlant(PlantType::PlantPeaShooter1, 0, 0)) { SDL_Log("add plant at (0, 0)\n"); }
    // if (0 == m_plantManager->addPlant(PlantType::PlantPeaShooter1, 0, 1)) { SDL_Log("add plant at (0, 1)\n"); }
    // if (0 == m_plantManager->addPlant(PlantType::PlantPeaShooter1, 1, 1)) { SDL_Log("add plant at (1, 1)\n"); }
    // if (0 == plant_manager->removePlant(1, 1)) { SDL_Log("remove plant at (1, 1)\n"); }

    // if (0 == m_zombieManager->addZombie(ZombieType::ZombieNormal, 0, 5)) { SDL_Log("add zombie at (0, 0)\n"); }
    // 相机位置
    CoreVars::getInstance().camera.setPosition(210.0f, 0.0f);
    return 0;
}

SceneType GameScene::handleEvent(SDL_Event& event)
{
    switch (event.type)
    {
    case SDL_KEYDOWN: {
        if (event.key.keysym.sym == SDLK_q)
            return SceneType::Scene_MainScene;
        break;
    }
    case SDL_MOUSEBUTTONDOWN: {
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            if (m_cardInHandIdx != -1 && m_plantInHandType != PlantType::MaxPlantType)  // 手中有植物
                _on_LB_haveCardInHand(event);   // 左键 尝试安防植物
            else
                _on_LB_noCardInHand(event); // 左键 尝试选卡 or 收集阳光/银币
        }
        else if (event.button.button == SDL_BUTTON_RIGHT)
        {
            _put_back_card(); // 右键放回植物
        }
        break;
    }
    case SDL_MOUSEMOTION: {
        // 坐标转换
        m_mousePositionX = event.motion.x;
        m_mousePositionY = event.motion.y;
        int mousePositionX = CoreVars::getInstance().camera.getClickX(event.motion.x);
        int mousePositionY = CoreVars::getInstance().camera.getClickY(event.motion.y);
        if (m_cardInHandIdx != -1) Managers::getInstance().plantManager->presettlePlant(mousePositionX, mousePositionY);
        break;
    }
    default:
        break;
    }
    return SceneType::Scene_MaxSceneIdx;
}

SceneType GameScene::updateScene()
{
    Managers::getInstance().taskManager->updateTask();
    Managers::getInstance().plantManager->updatePlants();
    Managers::getInstance().productManager->update();
    Managers::getInstance().zombieManager->updateZombie();
    Managers::getInstance().cardManager->updateCardInSlot();
    if (m_dropSunCountDown <= CoreVars::getInstance().timer.getDeltaTime())
    {
        m_dropSunCountDown = m_dropSunIntervalTime;
        Managers::getInstance().productManager->randomDropSun();
    }
    else
    {
        m_dropSunCountDown -= CoreVars::getInstance().timer.getDeltaTime();
    }
    if (Managers::getInstance().taskManager->isFinish() && !Managers::getInstance().zombieManager->hasZombie())
    {
        // win
        return SceneType::Scene_WinScene;
    }
    else if (Managers::getInstance().zombieManager->hasZombieInHouse())
    {
        // lose
        return SceneType::Scene_LoseScene;
    }
    return SceneType::Scene_MaxSceneIdx;
}

int GameScene::exitScene()
{
    SDL_Log("exit game scene\n");
    Managers::getInstance().productManager->collect();
    Managers::getInstance().productManager->setSunNum(50);
    return 0;
}

int GameScene::renderScene()
{
    Managers::getInstance().mapManager->renderMap();
    Managers::getInstance().plantManager->renderPlants();
    Managers::getInstance().zombieManager->renderZombie();
    Managers::getInstance().cardManager->renderCardSlot();
    if (m_cardInHandIdx != -1 && m_plantInHandType != PlantType::MaxPlantType)
    {
        // 渲染手中植物
        Managers::getInstance().cardManager->renderCardInHand(m_plantInHandType, m_mousePositionX, m_mousePositionY);
        // 预放置位置虚影(在 m_plantManager->renderPlants() 中实现)
    }
    Managers::getInstance().cardManager->renderCardCoolDown();
    Managers::getInstance().productManager->render();
    Managers::getInstance().taskManager->renderTask();
    return 0;
}

GameScene::~GameScene()
{}

void GameScene::_settle_plant()
{
    Managers::getInstance().plantManager->settlePlant();
    Managers::getInstance().cardManager->settleCard(m_cardInHandIdx);
    m_cardInHandIdx = -1;
    m_plantInHandType = PlantType::MaxPlantType;
}

void GameScene::_put_back_card()
{
    Managers::getInstance().cardManager->putbackCard(m_cardInHandIdx);
    m_cardInHandIdx = -1;
    m_plantInHandType = PlantType::MaxPlantType;
    Managers::getInstance().plantManager->putbackPlant();
}

void GameScene::_on_LB_haveCardInHand(SDL_Event& event)
{
    // 点击的卡槽
    if (Managers::getInstance().cardManager->inSlot(event.button.x, event.button.y))
    {
        _put_back_card();
    }
    // 点击非卡槽
    else
    {
        // 坐标转换
        int mouse_click_x = CoreVars::getInstance().camera.getClickX(event.button.x);
        int mouse_click_y = CoreVars::getInstance().camera.getClickY(event.button.y);
        int row = Managers::getInstance().mapManager->caculRow(mouse_click_y);
        int col = Managers::getInstance().mapManager->caculCol(mouse_click_x);
        if (Managers::getInstance().mapManager->isValidCell(row, col)
            && Managers::getInstance().plantManager->emptyPosition(m_plantInHandType, row, col))
        {
            // 安放植物
            _settle_plant();
        }
    }
}

void GameScene::_on_LB_noCardInHand(SDL_Event& event)
{
    // 收集
    int rt = Managers::getInstance().productManager->clickCollection(event.button.x, event.button.y);
    if (rt == 0) return;
    // 选卡
    m_cardInHandIdx = Managers::getInstance().cardManager->getSlotIdx(event.button.x, event.button.y);
    if (m_cardInHandIdx == -1) return;
    m_plantInHandType = Managers::getInstance().cardManager->pickupCard(m_cardInHandIdx);
    Managers::getInstance().plantManager->pickPlant(m_plantInHandType);
}