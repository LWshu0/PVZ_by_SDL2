#ifndef COLLECTIONMANAGER_H
#define COLLECTIONMANAGER_H

#include "Collection/CollectionObject.h"
#include "Collection/Sun.h"

#include <list>

class MapManager;

class CollectionManager
{
protected:
    // 不可变
    std::vector<std::shared_ptr<CollectionObject>> m_collectionTemplate;    // 掉落物模板

    // 游戏中的掉落物
    std::list<std::shared_ptr<CollectionObject>> m_collectionItems;
    // 允许存在在游戏内的掉落物最大数量
    int m_maxCollectionLimit;

    // 数据记录
    int m_sunNum;

    // 关联 manager
    std::shared_ptr<MapManager> m_mapManager;

public:
    CollectionManager(
        int maxCollection
    );

    // 关联 manager
    // 初始化 texture(卡片纹理与植物纹理)
    int initilizeManagers(
        std::shared_ptr<MapManager> mapManager
    );

    inline void setSunNum(int sum_num) { m_sunNum = sum_num; }
    inline int getSunNum() { return m_sunNum; }
    
    int addCollection(CollectionType type, int x, int y);

    int randomDropSun();

    // 生产得到的掉落物 抛物线形式的运动
    int produceCollection(CollectionType type, int x, int y);

    int clickCollection(int mouse_x, int mouse_y);
    
    int updateCollection();
    
    int renderCollection();

    int clearCollection();

    int collectCollection();

    void releaseManagers();

    ~CollectionManager();
};

#endif