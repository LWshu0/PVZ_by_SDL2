#include "Core/Particle/Particle_2d.h"
#include "Core/CoreVars.h"

ParticleEmitter_2d::ParticleEmitter_2d(
    float x, float y,
    uint64_t max_particle,
    int64_t life_time,
    uint64_t emit_delta_time,
    const std::shared_ptr<ParticleSetter_2d>& setter,
    const std::shared_ptr<ParticleUpdater_2d>& updater,
    const std::shared_ptr<ParticleRenderer_2d>& renderer
) :
    m_oneshoot(false),
    m_center{ x, y },
    m_lifetime(life_time),
    m_totalLifetime(life_time),
    m_maxParticleNum(max_particle),
    m_lastGenerateIndex(0),
    m_emitTimePassby(MAX_EMIT_MS),
    m_emitDeltaTime(emit_delta_time),
    m_setter(setter),
    m_renderer(renderer)
{
    if (m_setter != nullptr && m_maxParticleNum != 0 && m_totalLifetime > 0)
    {
        m_valid = true;
        m_particles.resize(m_maxParticleNum);
        m_updater.push_back(updater);
    }
    else m_valid = false;
}

ParticleEmitter_2d::ParticleEmitter_2d(
    float x, float y,
    uint64_t max_particle,
    int64_t life_time,
    uint64_t emit_delta_time,
    const std::shared_ptr<ParticleSetter_2d>& setter,
    const std::initializer_list<std::shared_ptr<ParticleUpdater_2d>>& updater,
    const std::shared_ptr<ParticleRenderer_2d>& renderer
) :
    m_oneshoot(false),
    m_center{ x, y },
    m_lifetime(life_time),
    m_totalLifetime(life_time),
    m_maxParticleNum(max_particle),
    m_lastGenerateIndex(0),
    m_emitTimePassby(MAX_EMIT_MS),
    m_emitDeltaTime(emit_delta_time),
    m_setter(setter),
    m_renderer(renderer)
{
    if (m_setter != nullptr && m_maxParticleNum != 0 && m_totalLifetime > 0)
    {
        m_valid = true;
        m_particles.resize(m_maxParticleNum);
        for (auto it : updater)
        {
            m_updater.push_back(it);
        }
    }
    else m_valid = false;
}

void ParticleEmitter_2d::initilize(float x, float y)
{
    m_center.x = x;
    m_center.y = y;
    m_lifetime = m_totalLifetime;
    m_valid = true;
    if (m_oneshoot)
    {
        for (uint32_t i = 0;i < m_maxParticleNum;i++)
        {
            m_setter->set(m_particles[i], *this);
        }
    }
}

void ParticleEmitter_2d::emit(uint32_t num)
{
    if (m_oneshoot) return;
    // 检查最早使用过的粒子
    for (uint32_t i = m_lastGenerateIndex;i < m_maxParticleNum && num > 0;i++)
    {
        if (!m_particles[i].alive())
        {
            m_setter->set(m_particles[i], *this);
            m_lastGenerateIndex = i;
            num--;
        }
    }
    // 从头开始检查
    for (uint32_t i = 0;i < m_lastGenerateIndex && num > 0;i++)
    {
        if (!m_particles[i].alive())
        {
            m_setter->set(m_particles[i], *this);
            m_lastGenerateIndex = i;
            num--;
        }
    }
    // 全部占满后 不进行添加
    // 应合理设置数组大小 避免出现这种情况
}

void ParticleEmitter_2d::update()
{
    if (!m_valid) return; // 失效的粒子发射器 不进行更新同时该粒子发射器需要被删除
    int count = 0;
    // 更新粒子
    for (auto& particle : m_particles)
    {
        if (particle.alive())
        {
            count++;
            for (auto it : m_updater)
            {
                it->update(particle);
            }
        }
    }
    // 更新粒子发射器
    m_lifetime -= CoreVars::getInstance().timer.getDeltaTime();
    if (0 == count && m_lifetime < 0) m_valid = false;
    else
    {
        m_emitTimePassby += CoreVars::getInstance().timer.getDeltaTime();
        if (m_emitTimePassby >= m_emitDeltaTime)
        {
            emit();
            m_emitTimePassby = 0;
        }
    }
}

void ParticleEmitter_2d::render()
{
    for (auto& particle : m_particles)
    {
        if (particle.alive())
        {
            m_renderer->render(particle);
        }
    }
}

ParticleEmitter_2d::~ParticleEmitter_2d()
{}