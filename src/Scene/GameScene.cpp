#include "Scene/GameScene.h"
#include "Manager/MapManager.h"
#include "Manager/BulletManager.h"
#include "Manager/PlantManager.h"
#include "Manager/ZombieManager.h"
#include "Manager/TaskManager.h"
#include "Manager/CardManager.h"
#include "Manager/CollectionManager.h"

GameScene::GameScene(
    std::shared_ptr<MapManager> mapManager,
    std::shared_ptr<BulletManager> bulletManager,
    std::shared_ptr<PlantManager> plantManager,
    std::shared_ptr<ZombieManager> zombieManager,
    std::shared_ptr<TaskManager> taskManager,
    std::shared_ptr<CardManager> cardManager,
    std::shared_ptr<CollectionManager> collectionManager
) :
    SceneObject(),
    m_cardInHandIdx(-1),
    m_plantInHandType(PlantType::MaxPlantType),
    m_dropSunIntervalTime(5000),
    m_mapManager(mapManager),
    m_bulletManager(bulletManager),
    m_plantManager(plantManager),
    m_zombieManager(zombieManager),
    m_taskManager(taskManager),
    m_cardManager(cardManager),
    m_collectionManager(collectionManager)
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
    m_mapManager->setMap(0.0f, 0.0f, MapType::MapGrassDayOneLine);
    m_plantManager->initilizePlants();
    m_zombieManager->initilizeZombie();
    m_bulletManager->clearBullets();
    m_taskManager->loadTask("task/1-1-1.xml");
    m_collectionManager->clearCollection();
    m_collectionManager->setSunNum(50);
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
                int row = m_mapManager->caculRow(mouse_click_y);
                int col = m_mapManager->caculCol(mouse_click_x);
                if (m_mapManager->isValidCell(row, col))
                {
                    // 安放植物
                    m_plantManager->settlePlant();
                    // m_plantManager->addPlant(m_plantInHandType, row, col);
                    m_cardManager->settleCard(m_cardInHandIdx);
                    m_cardInHandIdx = -1;
                    m_plantInHandType = PlantType::MaxPlantType;
                }
            }
            else if (event.button.button == SDL_BUTTON_RIGHT) // 右键放回植物
            {
                m_cardManager->putbackCard(m_cardInHandIdx);
                m_cardInHandIdx = -1;
                m_plantInHandType = PlantType::MaxPlantType;
                m_plantManager->putbackPlant();
            }
        }
        // 是否 pick 卡
        else    // 手中没有植物
        {
            if (event.button.button == SDL_BUTTON_LEFT) // 左键 尝试选卡 or 收集阳光/银币
            {
                m_cardInHandIdx = m_cardManager->getSlotIdx(event.button.x, event.button.y);
                if (m_cardInHandIdx != -1)
                {
                    m_plantInHandType = m_cardManager->pickupCard(m_cardInHandIdx);
                    m_plantManager->pickPlant(m_plantInHandType);
                }
                else // 尝试收集
                {
                    // todo ...
                    m_collectionManager->clickCollection(event.button.x, event.button.y);
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
        if (m_cardInHandIdx != -1) m_plantManager->presettlePlant(mousePositionX, mousePositionY);
    }
    return SceneType::Scene_MaxSceneIdx;
}

SceneType GameScene::updateScene()
{
    m_taskManager->updateTask();
    m_plantManager->updatePlants();
    m_bulletManager->updateBullets();
    m_zombieManager->updateZombie();
    m_zombieManager->attackPlants();
    m_cardManager->updateCardInSlot();
    m_collectionManager->updateCollection();
    if (m_dropSunCountDown <= GlobalVars::getInstance().timer.getDeltaTime())
    {
        m_dropSunCountDown = m_dropSunIntervalTime;
        m_collectionManager->randomDropSun();
    }
    else
    {
        m_dropSunCountDown -= GlobalVars::getInstance().timer.getDeltaTime();
    }
    if (m_taskManager->isFinish() && !m_zombieManager->hasZombie())
    {
        // win
        return SceneType::Scene_MainScene;
    }
    else if (m_zombieManager->hasZombieInHouse())
    {
        // lose
        return SceneType::Scene_MainScene;
    }
    return SceneType::Scene_MaxSceneIdx;
}

int GameScene::exitScene()
{
    SDL_Log("exit game scene\n");
    m_collectionManager->collectCollection();
    m_collectionManager->setSunNum(50);
    return 0;
}

int GameScene::renderScene()
{
    m_mapManager->renderMap();
    m_plantManager->renderPlants();
    m_zombieManager->renderZombie();
    m_bulletManager->renderBullets();
    m_cardManager->renderCardSlot();
    if (m_cardInHandIdx != -1 && m_plantInHandType != PlantType::MaxPlantType)
    {
        // 渲染手中植物
        m_cardManager->renderCardInHand(m_plantInHandType, m_mousePositionX, m_mousePositionY);
        // 预放置位置虚影(在 m_plantManager->renderPlants() 中实现)
    }
    m_cardManager->renderCardCoolDown();
    m_collectionManager->renderCollection();
    return 0;
}

GameScene::~GameScene()
{}