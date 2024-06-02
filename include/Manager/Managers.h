#ifndef MANAGERS_H
#define MANAGERS_H

#include <memory>

class MapManager;
class ProductManager;
class PlantManager;
class ZombieManager;
class TaskManager;
class CardManager;
class SceneManager;
class UsrInfoManager;

class Managers {
public:
    std::shared_ptr<MapManager> mapManager;
    std::shared_ptr<ProductManager> productManager;
    std::shared_ptr<PlantManager> plantManager;
    std::shared_ptr<ZombieManager> zombieManager;
    std::shared_ptr<TaskManager> taskManager;
    std::shared_ptr<CardManager> cardManager;
    std::shared_ptr<SceneManager> sceneManager;
    std::shared_ptr<UsrInfoManager> usrinfoManager;

    static Managers& getInstance();

    // 禁止复制和赋值
    Managers(Managers const&) = delete;
    void operator=(Managers const&) = delete;

private:
    Managers() = default;
    
};

#endif