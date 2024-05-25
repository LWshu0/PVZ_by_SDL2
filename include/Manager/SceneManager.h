#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H
#include "Core/Timer.h"
#include "Core/Camera.h"
#include "Scene/MainScene.h"
#include "Scene/SelectCardScene.h"
#include "Scene/Select2Game.h"
#include "Scene/GameScene.h"

class SceneManager {
protected:

    std::shared_ptr<SceneObject> m_currentScene;                // 当前所处的场景

    std::vector<std::shared_ptr<SceneObject>> m_sceneTemplate;  // 所有的场景

public:
    SceneManager(
        std::shared_ptr<MapManager> mapManager,
        std::shared_ptr<BulletManager> bulletManager,
        std::shared_ptr<PlantManager> plantManager,
        std::shared_ptr<ZombieManager> zombieManager,
        std::shared_ptr<TaskManager> taskManager,
        std::shared_ptr<CardManager> cardManager,
        std::shared_ptr<CollectionManager> collectionManager
        );

    // 调用当前场景的处理事件函数
    int handleEvent(SDL_Event& event);

    // 更新当前场景
    int updateScene();

    // 渲染当前的场景
    int renderScene();

    ~SceneManager();
};

#endif