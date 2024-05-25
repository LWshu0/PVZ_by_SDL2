#include "Collection/Sun.h"
/*
0: Sun3 -> range: [0, 12] total: [0, 12]
1: Sun2 -> range: [0, 12] total: [0, 12]
2: Sun1 -> range: [0, 12] total: [0, 12]
*/
Sun::Sun(
    const SDL_FRect& aabb,
    std::shared_ptr<AnimLoader> loader
) :
    CollectionObject(aabb),
    AnimPlayer(
        loader,
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
    return std::make_shared<Sun>(SDL_FRect{ x, y, m_aabb.w, m_aabb.h }, m_loader);
}

CollectionType Sun::getType()
{
    return CollectionType::Collection_Sun;
}

int Sun::updateCollection()
{
    updatePlayingFrameIdx(GlobalVars::getInstance().timer.getTime());
    m_updater->step(this, GlobalVars::getInstance().timer.getDeltaTime());
    setPlayPosition(SDL_FPoint{ m_aabb.x + m_aabb.w / 2 , m_aabb.y + m_aabb.h / 2 });
    return 0;
}

int Sun::renderCollection()
{
    render();
    SDL_SetRenderDrawColor(GlobalVars::getInstance().renderer, 255, 255, 255, 255);
    SDL_FRect render_aabb = m_aabb;
    render_aabb.x = GlobalVars::getInstance().camera.getRenderX(m_aabb.x);
    render_aabb.y = GlobalVars::getInstance().camera.getRenderY(m_aabb.y);
    SDL_RenderDrawRectF(GlobalVars::getInstance().renderer, &render_aabb);
    return 0;
}

Sun::~Sun()
{}
