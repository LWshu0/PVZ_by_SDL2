#include "Bullet/Pea.h"

Pea::Pea(
    SDL_Texture* bullet_texture,
    SDL_Texture* shadow_texture,
    float x, float y
) :
    BulletObject(
        SDL_FRect{ x, y, 20, 20 },  // AABB
        shadow_texture,             // 阴影纹理
        SDL_FPoint{ -10, 50 },      // 阴影偏移
        50, 20,                     // 阴影宽高        
        20                          // 伤害
    ),
    m_texture(bullet_texture)
{
    if (bullet_texture != nullptr)
    {
        SDL_QueryTexture(m_texture, NULL, NULL, &m_textureWidth, &m_textureHeight);
    }
}

std::shared_ptr<BulletObject> Pea::cloneBullet(float x, float y)
{
    std::shared_ptr<BulletObject> new_one = std::make_shared<Pea>(
        m_texture,
        m_shadow,
        x, y);
    new_one->setMotion(m_updater);
    return new_one;
}

int Pea::render()
{
    SDL_FRect tex_rect{ GlobalVars::getInstance().camera.getRenderX(m_aabb.x), GlobalVars::getInstance().camera.getRenderY(m_aabb.y), (float)m_textureWidth, (float)m_textureHeight };
    SDL_RenderCopyF(GlobalVars::getInstance().renderer, m_texture, NULL, &tex_rect);
    SDL_FRect sha_rect{ GlobalVars::getInstance().camera.getRenderX(m_aabb.x + m_offsetShadow.x), GlobalVars::getInstance().camera.getRenderY(m_aabb.y + m_offsetShadow.y), m_shadowWidth, m_shadowHeight };
    SDL_RenderCopyF(GlobalVars::getInstance().renderer, m_shadow, NULL, &sha_rect);
    SDL_FRect aabb_box = m_aabb;
    aabb_box.x = GlobalVars::getInstance().camera.getRenderX(aabb_box.x);
    aabb_box.y = GlobalVars::getInstance().camera.getRenderY(aabb_box.y);
    SDL_RenderDrawRectF(GlobalVars::getInstance().renderer, &aabb_box);

    return 0;
}