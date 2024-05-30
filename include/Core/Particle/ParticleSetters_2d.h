#ifndef PARTICLEBUILDERS_2D_H
#define PARTICLEBUILDERS_2D_H

#include "Particle_2d.h"

class ParticleSetter_2d_default : public ParticleSetter_2d {
protected:
    uint64_t m_initLifetime;
    SDL_FPoint m_initSpeed;
public:
    ParticleSetter_2d_default(uint64_t init_lifetime, const SDL_FPoint& speed = SDL_FPoint{ 0.0f, 0.0f });
    virtual void set(Particle_2d& particle, ParticleEmitter_2d& emitter) override;
    virtual ~ParticleSetter_2d_default();
};

class ParticleSetter_2d_explosion :public ParticleSetter_2d {
protected:
    uint64_t m_initLifetime;
    float m_initSpeed;
public:
    ParticleSetter_2d_explosion(uint64_t init_lifetime, float init_speed);
    virtual void set(Particle_2d& particle, ParticleEmitter_2d& emitter) override;
    virtual ~ParticleSetter_2d_explosion();
};

#endif