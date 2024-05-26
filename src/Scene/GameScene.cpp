#include "Scene/GameScene.h"
#include "Manager/MapManager.h"
#include "Manager/ProductManager.h"
#include "Manager/PlantManager.h"
#include "Manager/ZombieManager.h"
#include "Manager/TaskManager.h"
#include "Manager/CardManager.h"
#include "Core/GlobalVars.h"

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
    GlobalVars::getInstance().mapManager->setMap(0.0f, 0.0f, MapType::MapGrassDayOneLine);
    GlobalVars::getInstance().plantManager->initilizePlants();
    GlobalVars::getInstance().zombieManager->initilizeZombie();
    GlobalVars::getInstance().productManager->clear();
    GlobalVars::getInstance().taskManager->loadTask("task/1-1-1.xml");
    GlobalVars::getInstance().productManager->setSunNum(50);
    // if (0 == m_plantManager->addPlant(PlantType::PlantPeaShooter1, 0, 0)) { SDL_Log("add plant at (0, 0)\n"); }
    // if (0 == m_plantManager->addPlant(PlantType::PlantPeaShooter1, 0, 1)) { SDL_Log("add plant at (0, 1)\n"); }
    // if (0 == m_plantManager->addPlant(PlantType::PlantPeaShooter1, 1, 1)) { SDL_Log("add plant at (1, 1)\n"); }
    // if (0 == plant_manager->removePlant(1, 1)) { SDL_Log("remove plant at (1, 1)\n"); }

    // if (0 == m_zombieManager->addZombie(ZombieType::ZombieNormal, 0, 5)) { SDL_Log("add zombie at (0, 0)\n"); }
    // 相机位置
    GlobalVars::getInstance().camera.setPosition(210.0f, 0.0f);
    return 0;
}

SceneType GameScene::handleEvent(SDL_Event& event)
{
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q) return SceneType::Scene_MainScene;
    if (event.type == SDL_MOUSEBUTTONDOWN)
    {
        // 是否放置植物
        if (m_cardInHandIdx != -1 && m_plantInHandType != PlantType::MaxPlantType)  // 手中有植物
        {
            if (event.button.button == SDL_BUTTON_LEFT) // 左键 尝试安防植物
            {
                // 坐标转换
                int mouse_click_x = GlobalVars::getInstance().camera.getClickX(event.button.x);
                int  mouse_click_y = GlobalVars::getInstance().camera.getClickY(event.button.y);
                int row = GlobalVars::getInstance().mapManager->caculRow(mouse_click_y);
                int col = GlobalVars::getInstance().mapManager->caculCol(mouse_click_x);
                if (GlobalVars::getInstance().mapManager->isValidCell(row, col))
                {
                    // 安放植物
                    GlobalVars::getInstance().plantManager->settlePlant();
                    // m_plantManager->addPlant(m_plantInHandType, row, col);
                    GlobalVars::getInstance().cardManager->settleCard(m_cardInHandIdx);
                    m_cardInHandIdx = -1;
                    m_plantInHandType = PlantType::MaxPlantType;
                }
            }
            else if (event.button.button == SDL_BUTTON_RIGHT) // 右键放回植物
            {
                GlobalVars::getInstance().cardManager->putbackCard(m_cardInHandIdx);
                m_cardInHandIdx = -1;
                m_plantInHandType = PlantType::MaxPlantType;
                GlobalVars::getInstance().plantManager->putbackPlant();
            }
        }
        // 是否 pick 卡
        else    // 手中没有植物
        {
            if (event.button.button == SDL_BUTTON_LEFT) // 左键 尝试选卡 or 收集阳光/银币
            {
                m_cardInHandIdx = GlobalVars::getInstance().cardManager->getSlotIdx(event.button.x, event.button.y);
                if (m_cardInHandIdx != -1)
                {
                    m_plantInHandType = GlobalVars::getInstance().cardManager->pickupCard(m_cardInHandIdx);
                    GlobalVars::getInstance().plantManager->pickPlant(m_plantInHandType);
                }
                else // 尝试收集
                {
                    // todo ...
                    GlobalVars::getInstance().productManager->clickCollection(event.button.x, event.button.y);
                }

            }
        }
    }
    else if (event.type == SDL_MOUSEMOTION)
    {
        // 坐标转换
        m_mousePositionX = event.motion.x;
        m_mousePositionY = event.motion.y;
        int mousePositionX = GlobalVars::getInstance().camera.getClickX(event.motion.x);
        int mousePositionY = GlobalVars::getInstance().camera.getClickY(event.motion.y);
        if (m_cardInHandIdx != -1) GlobalVars::getInstance().plantManager->presettlePlant(mousePositionX, mousePositionY);
    }
    return SceneType::Scene_MaxSceneIdx;
}

SceneType GameScene::updateScene()
{
    GlobalVars::getInstance().taskManager->updateTask();
    GlobalVars::getInstance().plantManager->updatePlants();
    GlobalVars::getInstance().productManager->update();
    GlobalVars::getInstance().zombieManager->updateZombie();
    GlobalVars::getInstance().cardManager->updateCardInSlot();
    if (m_dropSunCountDown <= GlobalVars::getInstance().timer.getDeltaTime())
    {
        m_dropSunCountDown = m_dropSunIntervalTime;
        GlobalVars::getInstance().productManager->randomDropSun();
    }
    else
    {
        m_dropSunCountDown -= GlobalVars::getInstance().timer.getDeltaTime();
    }
    if (GlobalVars::getInstance().taskManager->isFinish() && !GlobalVars::getInstance().zombieManager->hasZombie())
    {
        // win
        return SceneType::Scene_WinScene;
    }
    else if (GlobalVars::getInstance().zombieManager->hasZombieInHouse())
    {
        // lose
        return SceneType::Scene_MainScene;
    }
    return SceneType::Scene_MaxSceneIdx;
}

int GameScene::exitScene()
{
    SDL_Log("exit game scene\n");
    GlobalVars::getInstance().productManager->collect();
    GlobalVars::getInstance().productManager->setSunNum(50);
    return 0;
}

int GameScene::renderScene()
{
    GlobalVars::getInstance().mapManager->renderMap();
    GlobalVars::getInstance().plantManager->renderPlants();
    GlobalVars::getInstance().zombieManager->renderZombie();
    GlobalVars::getInstance().productManager->render();
    GlobalVars::getInstance().cardManager->renderCardSlot();
    if (m_cardInHandIdx != -1 && m_plantInHandType != PlantType::MaxPlantType)
    {
        // 渲染手中植物
        GlobalVars::getInstance().cardManager->renderCardInHand(m_plantInHandType, m_mousePositionX, m_mousePositionY);
        // 预放置位置虚影(在 m_plantManager->renderPlants() 中实现)
    }
    GlobalVars::getInstance().cardManager->renderCardCoolDown();
    return 0;
}

GameScene::~GameScene()
{}