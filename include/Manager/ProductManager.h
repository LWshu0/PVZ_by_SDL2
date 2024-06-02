#ifndef PRODUCTMANAGER_H
#define PRODUCTMANAGER_H

#include "Product/ProductObject.h"
#include "Product/BulletObject.h"
#include "Product/CollectionObject.h"
#include "Product/Pea.h"
#include "Product/Sun.h"

#include <list>

#include "Manager/Managers.h"

class ProductManager {
protected:
    std::vector<std::shared_ptr<ObjectPool<ProductObject>>> m_productPool;
    // 允许存在在游戏内的掉落物最大数量
    int m_maxCollectionLimit;
    std::list<std::shared_ptr<CollectionObject>> m_collectionItems;

    // 允许存在在游戏内的子弹最大数量
    int m_maxBulletLimit;
    std::list<std::shared_ptr<BulletObject>> m_bulletItems;

    // 游戏记录
    int m_sunNum;

public:
    ProductManager(
        int maxCollection,
        int maxBullet
    );

    inline void setSunNum(int sum_num) { m_sunNum = sum_num; }
    inline int getSunNum() { return m_sunNum; }

    // 添加一个物体 按照模板的运动方式
    int addBullet(ProductType type, int x, int y);
    int addCollection(ProductType type, int x, int y);
    // 生产得到的掉落物 抛物线形式的运动
    int produceCollection(ProductType type, int x, int y);
    // 随机掉落阳光
    int randomDropSun();

    // 计算子弹伤害
    int calculateDamage(std::shared_ptr<GameObject> other);
    // 鼠标点击收集
    int clickCollection(int mouse_x, int mouse_y);
    // 更新
    int update();
    // 渲染
    int render();
    // 清空
    int clear();
    // 收集所有
    int collect();

    ~ProductManager();
};

#endif