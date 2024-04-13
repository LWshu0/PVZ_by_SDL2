#ifndef BULLETMANAGER_H
#define BULLETMANAGER_H

#include "Core/TextureRes.h"
#include "Core/Camera.h"
#include "Core/ObjectMotion.h"
#include "Bullet/Pea.h"
#include <vector>
#include <memory>

class BulletManager
{
protected:
    // 不可变
    SDL_Renderer* m_renderer;
    std::shared_ptr<TextureRes> m_textureRes;
    std::shared_ptr<Camera> m_camera;
    std::vector<std::shared_ptr<BulletObject>> m_bulletTemplate;
    // 可变
    int m_maxBulletNum;
    std::vector<std::shared_ptr<BulletObject>> m_bullets;

public:
    BulletManager(
        SDL_Renderer* renderer,
        std::shared_ptr<TextureRes> texture_res,
        std::shared_ptr<Camera> camera,
        int maxBullet);

    int addBullet(BulletType type, float x, float y);

    int updateBullets(uint64_t delta_ms);

    int renderBullets();

    int clearBullets();

    ~BulletManager();
};

#endif