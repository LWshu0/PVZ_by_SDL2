#ifndef ZOMBIEMANAGER_H
#define ZOMBIEMANAGER_H

#include <vector>
#include <memory>

#include "Core/GlobalVars.h"
#include "Core/AnimLoader.h"

#include "Zombies/ZombieObject.h"
#include "Zombies/Zombie.h"
#include "Plants/PlantObject.h"

class MapManager;
class ProductManager;
class PlantManager;

class ZombieManager {
protected:
    /**************************
    *    初始化后不可变成员   *
    **************************/
    std::vector<std::shared_ptr<AnimLoader>> m_animLoader;          // 僵尸动画资源
    std::vector<std::shared_ptr<ZombieObject>> m_zombieTemplate;    // 僵尸模板

    // zombies in game
    int m_zombieNum;
    std::vector<std::shared_ptr<ZombieObject>> m_zombies;

public:
    ZombieManager();

    // 清空场上所有僵尸
    int initilizeZombie();

    // 添加一个僵尸在指定的行列
    // 当列号可以超过地图最大的列数, 当超出时, 将放置在屏幕外指定的位置(原版游戏中的效果)
    int addZombie(ZombieType type, int row, int col);

    // 在第 row 行的 left_x 和 right_x 之间是否有僵尸
    // 其中 left_x 和 right_x 都是世界坐标系下的坐标
    bool hasZombieBetween(int row, float left_x, float right_x);
    bool hasZombieInAttackRange(std::shared_ptr<PlantObject> plant);
    inline bool hasZombie() { return m_zombieNum > 0; }

    bool hasZombieInHouse();
    
    // 僵尸移动
    // 与子弹的碰撞检测, 并减少僵尸生命值
    int updateZombie();

    // 攻击植物检测
    // 当出现植物时转为攻击状态, 否则转为行走状态
    int attackPlants();

    // 渲染僵尸
    int renderZombie();

    ~ZombieManager();

    // 测试(弃用)
    int changeAllTo(ZombieState state);
};

#endif