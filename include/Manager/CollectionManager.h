#ifndef COLLECTIONMANAGER_H
#define COLLECTIONMANAGER_H

#include "Collection/CollectionObject.h"
#include "Collection/Sun.h"

class CollectionManager
{
protected:
    // 不可变
    SDL_Renderer* m_renderer;                                               // 渲染器
    std::shared_ptr<TextureRes> m_textureRes;                               // 纹理资源
    std::shared_ptr<Camera> m_camera;                                       // 相机
    std::shared_ptr<Timer> m_timer;                                         // 游戏时钟
    std::vector<std::shared_ptr<CollectionObject>> m_collectionTemplate;    // 子弹模板

    std::shared_ptr<ObjectMotion> m_dropMotion;
    // 游戏中的掉落物
    std::vector<std::shared_ptr<CollectionObject>> m_collectionItems;

    // 数据记录
    int m_sunNum;

public:
    CollectionManager(
        SDL_Renderer* renderer,
        std::shared_ptr<TextureRes> texture_res,
        std::shared_ptr<Camera> camera,
        std::shared_ptr<Timer> timer,
        int maxCollection
    );

    int addCollection(CollectionType type, int x, int y);

    int clickCollection(int mouse_x, int mouse_y);
    
    int updateCollection();
    
    int renderCollection();

    int clearCollection();

    int collectCollection();

    ~CollectionManager();
};

#endif