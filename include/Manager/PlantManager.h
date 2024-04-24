#ifndef PLANTMANAGER_H
#define PLANTMANAGER_H

#include <vector>
#include <memory>

#include "Plants/PlantObject.h"
#include "Plants/PeaShooterSingle.h"

class MapManager;
class BulletManager;
class ZombieManager;

class PlantManager {
protected:
    /**************************
    *    初始化后不可变成员   *
    **************************/
    SDL_Renderer* m_renderer;                                   // 渲染器
    std::shared_ptr<TextureRes> m_textureRes;                   // 纹理资源
    std::shared_ptr<Camera> m_camera;                           // 相机
    std::shared_ptr<Timer> m_timer;                             // 游戏时钟
    std::vector<std::shared_ptr<AnimLoader>> m_animLoader;      // 植物动画资源
    std::vector<std::shared_ptr<PlantObject>> m_plantTemplate;  // 植物模板

    // 关联 manager
    std::shared_ptr<MapManager> m_mapManager;
    std::shared_ptr<BulletManager> m_bulletManager;
    std::shared_ptr<ZombieManager> m_zombieManager;
    // plants in game
    std::vector<std::vector<std::shared_ptr<PlantObject>>> m_mainPlants;
    // 指示植物放置位置
    // 如果为 nullptr 则没有在手中的植物
    int m_presettleRowIdx, m_presettleColIdx;   // 预放置植物的位置
    std::shared_ptr<PlantObject> m_presettlePlantImage;
public:
    PlantManager(
        SDL_Renderer* renderer,
        std::shared_ptr<TextureRes> res,
        std::shared_ptr<Camera> camera,
        std::shared_ptr<Timer> timer
    );

    int initilizeManagers(
        std::shared_ptr<MapManager> mapManager,
        std::shared_ptr<BulletManager> bulletManager,
        std::shared_ptr<ZombieManager> zombieManager
    );

    int initilizePlants();

    bool collisionPlant(std::shared_ptr<GameObject> obj, int row, int col);

    // 游戏中拿起卡片时, 创建一个植物
    // 存放在 m_presettlePlantImage 中
    // 如果手中已有植物 返回 -1, 成功则返回 0
    int pickPlant(PlantType type);
    // 预放置植物到指定的行列
    // 仅修改 m_presettlePlantImage 的位置
    int presettlePlant(int mouse_x, int mouse_y);
    // 安置手中的植物到地图中指定位置, m_presettlePlantImage 指向的
    // 安置后 m_presettlePlantImage 置为 nullptr
    int settlePlant();
    // 将植物放回卡槽中后, m_presettlePlantImage 置为 nullptr
    // 即删除手中的植物
    int putbackPlant();

    int addPlant(PlantType type, int row, int col);
    int removePlant(int row, int col);
    int doDamage(int row, int col, int d);

    int updatePlants();
    int renderPlants();

    void releaseManagers();

    ~PlantManager();

    // 根据植物类型枚举获取 loader
    inline std::shared_ptr<AnimLoader> getPlantAnimLoader(int plant_type)
    {
        return plant_type == PlantType::MaxPlantType ? nullptr : m_animLoader[plant_type];
    }

    // 根据植物类型枚举获取 plant 模板
    inline std::shared_ptr<PlantObject> getPlantTemplate(int plant_type)
    {
        return plant_type == PlantType::MaxPlantType ? nullptr : m_plantTemplate[plant_type];
    }

    // 测试
    int changeAllTo(PlantState state);
};

#endif