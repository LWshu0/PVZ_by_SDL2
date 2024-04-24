#ifndef BULLETMANAGER_H
#define BULLETMANAGER_H

#include <vector>
#include <memory>
#include "Core/TextureRes.h"
#include "Core/Camera.h"
#include "Core/Timer.h"
#include "Core/ObjectMotion.h"

#include "Bullet/Pea.h"

class BulletManager {
protected:
    // 不可变
    SDL_Renderer* m_renderer;                                       // 渲染器
    std::shared_ptr<TextureRes> m_textureRes;                       // 纹理资源
    std::shared_ptr<Camera> m_camera;                               // 相机
    std::shared_ptr<Timer> m_timer;                                 // 游戏时钟
    std::vector<std::shared_ptr<BulletObject>> m_bulletTemplate;    // 子弹模板
    // 可变
    int m_maxBulletNum;                                             // 局内最大子弹数量
    std::vector<std::shared_ptr<BulletObject>> m_bullets;           // 子弹数组

public:
    BulletManager(
        SDL_Renderer* renderer,
        std::shared_ptr<TextureRes> texture_res,
        std::shared_ptr<Camera> camera,
        std::shared_ptr<Timer> timer,
        int maxBullet
    );

    // 添加一个子弹在指定的位置(世界坐标系)
    int addBullet(BulletType type, float x, float y);

    // 计算与 other 碰撞的 damage
    // 删除碰撞后的 bullet
    int collisionBullet(GameObject& other);
    int collisionBullet(std::shared_ptr<GameObject> other);

    // 更新所有的子弹
    // 运动与动画等
    int updateBullets();

    // 渲染所有的子弹
    int renderBullets();

    // 清空子弹 在进入游戏场景中时需要调用该函数
    int clearBullets();

    ~BulletManager();
};

#endif