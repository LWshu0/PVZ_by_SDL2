#include "Product/Sun.h"
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
    m_animPlayer(
        loader,
        SDL_FPoint{ aabb.x + aabb.w / 2 , aabb.y + aabb.h / 2 }   // 渲染位置
    )
{
    m_animPlayer.setPlayingTrack(
        { 0, 1, 2 },
        0
    );
    m_animPlayer.restartTrack(
        { 0, 1, 2 }
    );
}

std::shared_ptr<ProductObject> Sun::clone(float x, float y)
{
    return std::make_shared<Sun>(SDL_FRect{ x, y, m_aabb.w, m_aabb.h }, m_animPlayer.getAnimLoader());
}

ProductType Sun::getType()
{
    return ProductType::SunType;
}

int Sun::update()
{
    m_animPlayer.updatePlayingFrameIdx();
    m_updater->step(this, GlobalVars::getInstance().timer.getDeltaTime());
    m_animPlayer.setPlayPosition(SDL_FPoint{ m_aabb.x + m_aabb.w / 2 , m_aabb.y + m_aabb.h / 2 });
    return 0;
}

int Sun::render()
{
    m_animPlayer.render();
#ifndef NDEBUG
    SDL_SetRenderDrawColor(GlobalVars::getInstance().renderer, 255, 255, 255, 255);
    SDL_FRect render_aabb = m_aabb;
    render_aabb.x = GlobalVars::getInstance().camera.getRenderX(m_aabb.x);
    render_aabb.y = GlobalVars::getInstance().camera.getRenderY(m_aabb.y);
    SDL_RenderDrawRectF(GlobalVars::getInstance().renderer, &render_aabb);
#endif
    return 0;
}

Sun::~Sun()
{}
