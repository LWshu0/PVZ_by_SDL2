#include "Bullet/Pea.h"

Pea::Pea(
    SDL_Renderer* renderer,
    SDL_Texture* texture,
    std::shared_ptr<Camera> camera,
    float x, float y
) :
    BulletObject(renderer, camera, SDL_FRect{ x, y, 20, 20 }, 20),
    m_texture(texture)
{
    if (texture != nullptr)
    {
        SDL_QueryTexture(m_texture, NULL, NULL, &m_textureWidth, &m_textureHeight);
    }
}

std::shared_ptr<BulletObject> Pea::cloneBullet(float x, float y)
{
    std::shared_ptr<BulletObject> new_one = std::make_shared<Pea>(
        m_renderer,
        m_texture,
        m_camera,
        x, y);
    new_one->setMotion(m_updater);
    return new_one;
}

int Pea::renderBullet()
{
    SDL_FRect tex_rect{ m_camera->getRenderX(m_aabb.x), m_camera->getRenderY(m_aabb.y), (float)m_textureWidth, (float)m_textureHeight };
    SDL_RenderCopyF(m_renderer, m_texture, NULL, &tex_rect);
    SDL_FRect aabb_box = m_aabb;
    aabb_box.x = m_camera->getRenderX(aabb_box.x);
    aabb_box.y = m_camera->getRenderY(aabb_box.y);
    SDL_RenderDrawRectF(m_renderer, &aabb_box);
    return 0;
}