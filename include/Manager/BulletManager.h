#ifndef BULLETMANAGER_H
#define BULLETMANAGER_H

#include "Core/ObjectMotion.h"
#include "Bullet/Pea.h"
#include <vector>
#include <memory>

class BulletManager
{
protected:
    SDL_Renderer* m_renderer;
    int m_maxBulletNum;
    std::vector<std::shared_ptr<BulletObject>> m_bullets;

public:
    BulletManager(SDL_Renderer* renderer, int maxBullet);

    int addBullet(std::shared_ptr<BulletObject> bulletTemplate, float x, float y);

    int updateBullets(uint64_t delta_ms);

    int renderBullets();

    ~BulletManager();
};

#endif