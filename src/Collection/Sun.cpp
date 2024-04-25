#include "Collection/Sun.h"

Sun::Sun(
    const SDL_FRect& aabb,
    std::shared_ptr<AnimLoader> loader,
    std::shared_ptr<Camera> camera
) :
    CollectionObject(aabb),
    AnimPlayer(
        loader, camera,
        SDL_FPoint{ aabb.x + aabb.w / 2 , aabb.y + aabb.h / 2 }   // 渲染位置
    )
{
    setPlayingTrack(
        { 0, 1, 2 },
        0
    );
    restartTrack(
        { 0, 1, 2 }
    );
}

std::shared_ptr<CollectionObject> Sun::cloneCollection(float x, float y)
{
    return std::make_shared<Sun>(SDL_FRect{ x, y, m_aabb.w, m_aabb.h }, m_loader, m_camera);
}

CollectionType Sun::getType()
{
    return CollectionType::Collection_Sun;
}

int Sun::updateCollection(std::shared_ptr<Timer> timer)
{
    updatePlayingFrameIdx(timer->getTime());
    m_updater->step(this, timer->getDeltaTime());
    setPlayPosition(SDL_FPoint{ m_aabb.x + m_aabb.w / 2 , m_aabb.y + m_aabb.h / 2 });
    return 0;
}

int Sun::renderCollection()
{
    render();
    SDL_SetRenderDrawColor(m_loader->m_renderer, 255, 255, 255, 255);
    SDL_FRect render_aabb = m_aabb;
    render_aabb.x = m_camera->getRenderX(m_aabb.x);
    render_aabb.y = m_camera->getRenderY(m_aabb.y);
    SDL_RenderDrawRectF(m_loader->m_renderer, &render_aabb);
    return 0;
}

Sun::~Sun()
{}
