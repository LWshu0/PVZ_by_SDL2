#include "Product/Sun.h"
#include "Core/CoreVars.h"
#include "Resource/ResVars.h"

/*
0: Sun3 -> range: [0, 12] total: [0, 12]
1: Sun2 -> range: [0, 12] total: [0, 12]
2: Sun1 -> range: [0, 12] total: [0, 12]
*/
Sun::Sun(
    const SDL_FRect& aabb
) :
    CollectionObject(aabb),
    m_animPlayer(
        ResVars::getInstance().animRes.getAnimFrom("reanim/Sun.reanim"),
        SDL_FPoint{ aabb.x + aabb.w / 2 , aabb.y + aabb.h / 2 }   // 渲染位置
    ),
    m_lifetimeMilliSecond(8000)
{
    m_animPlayer.setPlayingTrack(
        { 0, 1, 2 },
        0
    );
    m_animPlayer.restartTrack(
        { 0, 1, 2 }
    );
}

void Sun::initilize(float x, float y)
{
    m_state = ProductState::Product_MOVE;
    m_lifetimeMilliSecond = 8000;
    m_aabb.x = x;
    m_aabb.y = y;
    m_animPlayer.setPlayPosition(SDL_FPoint{ m_aabb.x + m_aabb.w / 2 , m_aabb.y + m_aabb.h / 2 });
    m_animPlayer.restartTrack(
        { 0, 1, 2 }
    );
}

ProductType Sun::getType()
{
    return ProductType::SunType;
}

int Sun::update()
{
    m_animPlayer.updatePlayingFrameIdx();
    if (m_lifetimeMilliSecond > CoreVars::getInstance().timer.getDeltaTime())
    {
        m_lifetimeMilliSecond -= CoreVars::getInstance().timer.getDeltaTime();
    }
    else
    {
        m_state = ProductState::Product_DELETE;
    }
    int rt = m_updater->step(this, CoreVars::getInstance().timer.getDeltaTime());
    m_animPlayer.setPlayPosition(SDL_FPoint{ m_aabb.x + m_aabb.w / 2 , m_aabb.y + m_aabb.h / 2 });
    if (rt == -1 && ProductState::Product_DEAD == m_state)
    {
        m_state = ProductState::Product_DELETE;
    }
    return 0;
}

int Sun::render()
{
    m_animPlayer.render();
#ifndef NDEBUG
    SDL_SetRenderDrawColor(CoreVars::getInstance().renderer, 255, 255, 255, 255);
    SDL_FRect render_aabb = m_aabb;
    render_aabb.x = CoreVars::getInstance().camera.getRenderX(m_aabb.x);
    render_aabb.y = CoreVars::getInstance().camera.getRenderY(m_aabb.y);
    SDL_RenderDrawRectF(CoreVars::getInstance().renderer, &render_aabb);
#endif
    return 0;
}

Sun::~Sun()
{}


SunFactory::SunFactory() {}

std::shared_ptr<ProductObject> SunFactory::create()
{
    return std::make_shared<Sun>(SDL_FRect{ 0.0f,0.0f, 80.0f,80.0f });
}