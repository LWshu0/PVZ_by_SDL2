#include "Product/Pea.h"
#include "Core/Particle/ParticleSetters_2d.h"
#include "Core/Particle/ParticleUpdaters_2d.h"
#include "Core/Particle/ParticleRenderers_2d.h"
#include "Manager/ZombieManager.h"
#include "Core/CoreVars.h"
#include "Resource/ResVars.h"

Pea::Pea(
    float x, float y,
    std::shared_ptr<ParticleEmitter_2d> centerEmitter,
    std::shared_ptr<ParticleEmitter_2d> otherEmitter
) :
    BulletObject(
        SDL_FRect{ x, y, 20, 20 },  // AABB
        SDL_FPoint{ -10, 50 },      // 阴影偏移
        50, 20,                     // 阴影宽高        
        20                          // 伤害
    ),
    m_centerEmitter(centerEmitter),
    m_otherEmitter(otherEmitter)
{
    m_texture = ResVars::getInstance().textureRes.getTextureFrom("images/ProjectilePea.png");
    if (m_texture != nullptr)
    {
        SDL_QueryTexture(m_texture, NULL, NULL, &m_textureWidth, &m_textureHeight);
    }
    m_updater = std::make_shared<MotionSpeed>(200.0f, 0.0f);
}

void Pea::initilize(float x, float y)
{
    m_state = ProductState::Product_MOVE;
    m_aabb.x = x;
    m_aabb.y = y;
}

ProductType Pea::getType() { return ProductType::PeaType; }

void Pea::setState(ProductState state)
{
    switch (state)
    {
    case ProductState::Product_DEAD:
        m_otherEmitter->initilize(m_aabb.x + m_aabb.w, m_aabb.y);
        m_centerEmitter->initilize(m_aabb.x + m_aabb.w, m_aabb.y);
        break;
    default: // remove state
        // do nothing
        break;
    }
    m_state = state;
}

int Pea::update()
{
    switch (m_state)
    {
    case ProductState::Product_MOVE:
        m_updater->step(this, CoreVars::getInstance().timer.getDeltaTime());
        if (0 != Managers::getInstance().zombieManager->damageZombie(this, m_damage))
        {
            setState(ProductState::Product_DEAD);
        }
        break;
    case ProductState::Product_DEAD:
        m_otherEmitter->update();
        m_centerEmitter->update();
        if (!m_otherEmitter->valid() && !m_centerEmitter->valid())
        {
            m_state = ProductState::Product_DELETE;
        }
        break;
    default:
        break;
    }
    return 0;
}

int Pea::render()
{
    switch (m_state)
    {
    case ProductState::Product_MOVE:
    {
        SDL_FRect tex_rect{ CoreVars::getInstance().camera.getRenderX(m_aabb.x), CoreVars::getInstance().camera.getRenderY(m_aabb.y), (float)m_textureWidth, (float)m_textureHeight };
        SDL_RenderCopyF(CoreVars::getInstance().renderer, m_texture, NULL, &tex_rect);
        SDL_FRect sha_rect{ CoreVars::getInstance().camera.getRenderX(m_aabb.x + m_offsetShadow.x), CoreVars::getInstance().camera.getRenderY(m_aabb.y + m_offsetShadow.y), m_shadowWidth, m_shadowHeight };
        SDL_RenderCopyF(CoreVars::getInstance().renderer, m_shadow, NULL, &sha_rect);
        break;
    }
    case ProductState::Product_DEAD:
    {
        m_otherEmitter->render();
        m_centerEmitter->render();
        break;
    }
    default:
        break;
    }
    
#ifndef NDEBUG
    SDL_FRect aabb_box = m_aabb;
    aabb_box.x = CoreVars::getInstance().camera.getRenderX(aabb_box.x);
    aabb_box.y = CoreVars::getInstance().camera.getRenderY(aabb_box.y);
    SDL_RenderDrawRectF(CoreVars::getInstance().renderer, &aabb_box);
#endif
    return 0;
}

PeaFactory::PeaFactory()
{
    // m_motionUpdater = std::make_shared<MotionSpeed>(200.0f, 0.0f);
    // builder
    setter_center = std::make_shared<ParticleSetter_2d_default>(
        200
    );
    setter_other = std::make_shared<ParticleSetter_2d_explosion>(
        300,
        100
    );
    // updater
    updater_lifetime = std::make_shared<ParticleUpdater_2d_Lifetime>();
    updater_speed = std::make_shared<ParticleUpdater_2d_Speed>();
    updater_size = std::make_shared<ParticleUpdater_2d_Size>(0.5f, 0.5f);
    updater_force = std::make_shared<ParticleUpdater_2d_Force>(0.0f, 150.0f);
    // renderer
    render_other = std::make_shared<ParticleRenderer_2d_Sprite>(
        ResVars::getInstance().textureRes.getTextureFrom("particles/Pea_particles.png"), 3);

    render_center = std::make_shared<ParticleRenderer_2d_Sprite>(
        ResVars::getInstance().textureRes.getTextureFrom("particles/pea_splats.png"), 4);

}

std::shared_ptr<ProductObject> PeaFactory::create()
{
    std::shared_ptr<ParticleEmitter_2d> emitter_center = std::make_shared<ParticleEmitter_2d>(
        0.0f, 0.0f,         // 发射器位置
        1,                    // 最大粒子数量
        10,            // 发射器生命值
        3000,                   // 发射间隔
        setter_center,               // 粒子构造器
        updater_lifetime,   // 粒子更新器
        render_center                 // 粒子渲染器
    );
    emitter_center->setOneShoot(true);

    std::shared_ptr<ParticleEmitter_2d> emitter_particle = std::make_shared<ParticleEmitter_2d>(
        0.0f, 0.0f,
        5,
        10,
        3000,
        setter_other,
        std::initializer_list<std::shared_ptr<ParticleUpdater_2d>>{ updater_lifetime, updater_speed, updater_force },
        render_other
    );
    emitter_particle->setOneShoot(true);

    return std::make_shared<Pea>(0.0f, 0.0f, emitter_center, emitter_particle);
}