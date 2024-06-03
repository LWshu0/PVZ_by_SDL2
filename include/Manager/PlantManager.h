#ifndef PLANTMANAGER_H
#define PLANTMANAGER_H

#include <vector>
#include <memory>
#include "Plants/PlantObject.h"
#include "Plants/PeaShooterSingle.h"
#include "Plants/SunFlower.h"
#include "Manager/Managers.h"

class PlantManager {
protected:
    /**************************
    *    初始化后不可变成员   *
    **************************/
    std::vector<std::shared_ptr<AnimLoader>> m_animLoader;      // 植物动画资源
    std::vector<std::shared_ptr<PlantObject>> m_plantTemplate;  // 植物模板

    // plants in game
    std::vector<std::vector<std::shared_ptr<PlantObject>>> m_mainPlants;
    // 指示植物放置位置
    // 如果为 nullptr 则没有在手中的植物
    int m_presettleRowIdx, m_presettleColIdx;   // 预放置植物的位置
    std::shared_ptr<PlantObject> m_presettlePlantImage;
public:
    PlantManager();

    // 根据地图大小(由地图管理者处获得)创建植物数组用于存储植物
    // 清空所有场上的植物
    // 预放置的植物设置为 nullptr(将会是半透明渲染的虚影)
    int initilizePlants();

    // 判断给定的 GameObject 是否和 row 行, col 列的植物碰撞
    bool collisionPlant(GameObject* obj, int row, int col);

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

    // 添加一个植物在指定的行列(不做地形检查)
    int addPlant(PlantType type, int row, int col);
    // 移除指定位置的植物
    int removePlant(int row, int col);
    // 对指定位置的植物造成伤害
    int doDamage(int row, int col, int d);
    // 更新所有植物(不包括 m_presettlePlantImage)
    int updatePlants();
    // 渲染所有植物(包括 m_presettlePlantImage)
    // 当 m_presettlePlantImage 有效时才渲染(行列不为 -1, 且 m_presettlePlantImage 不为 nullptr)
    int renderPlants();

    ~PlantManager();

    bool emptyPosition(int plant_type, int row, int col);

    // 根据植物类型枚举获取 loader
    inline std::shared_ptr<AnimLoader> getPlantAnimLoader(int plant_type)
    {
        return plant_type == PlantType::MaxPlantType ? nullptr : m_animLoader[plant_type];
    }

    // 根据植物类型枚举获取 plant 模板
    // 在创建卡片纹理时使用, 用以访问指定植物的 renderStatic 函数
    inline std::shared_ptr<PlantObject> getPlantTemplate(int plant_type)
    {
        return plant_type == PlantType::MaxPlantType ? nullptr : m_plantTemplate[plant_type];
    }

    // 测试(弃用)
    int changeAllTo(PlantState state);
};

#endif