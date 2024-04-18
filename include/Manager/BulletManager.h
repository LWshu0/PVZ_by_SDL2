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

    int addBullet(BulletType type, float x, float y);

    // 计算与 other 碰撞的 damage
    // 删除碰撞后的 bullet
    int collisionBullet(GameObject& other);
    int collisionBullet(std::shared_ptr<GameObject> other);

    int updateBullets();

    int renderBullets();

    int clearBullets();

    ~BulletManager();
};

#endif