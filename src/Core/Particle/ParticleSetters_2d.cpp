#include "Core/Particle/ParticleSetters_2d.h"

ParticleSetter_2d_default::ParticleSetter_2d_default(uint64_t init_lifetime, const SDL_FPoint& speed) :
    m_initLifetime(init_lifetime), m_initSpeed(speed)
{}
void ParticleSetter_2d_default::set(Particle_2d& particle, ParticleEmitter_2d& emitter)
{
    // position
    particle.m_originPoint = emitter.m_center;
    particle.m_color = SDL_Color{ 255,255,255,255 };
    particle.m_scale = 1.0f;
    particle.m_degree = 0.0f;
    // lifetime
    particle.m_lifetime = m_initLifetime;
    particle.m_totalLifetime = m_initLifetime;
    // speed
    particle.m_speed = m_initSpeed;
}
ParticleSetter_2d_default::~ParticleSetter_2d_default() {}

ParticleSetter_2d_explosion::ParticleSetter_2d_explosion(uint64_t init_lifetime, float init_speed) :
    ParticleSetter_2d(),
    m_initLifetime(init_lifetime),
    m_initSpeed(init_speed)
{}

void ParticleSetter_2d_explosion::set(Particle_2d& particle, ParticleEmitter_2d& emitter)
{
    // position
    particle.m_originPoint = emitter.m_center;
    particle.m_color = SDL_Color{ 255,255,255,255 };
    particle.m_scale = 1.0f;
    particle.m_degree = 0.0f;
    // lifetime
    particle.m_lifetime = m_initLifetime;
    particle.m_totalLifetime = m_initLifetime;
    // speed
    float r = M_PI * 2 * (float)rand() / RAND_MAX;
    particle.m_speed = SDL_FPoint{ m_initSpeed * std::sin(r), m_initSpeed * std::cos(r) };
}

ParticleSetter_2d_explosion::~ParticleSetter_2d_explosion()
{

}