#include "Core/Particle/ParticleUpdaters_2d.h"
#include "Core/GlobalVars.h"

ParticleUpdater_2d_Lifetime::ParticleUpdater_2d_Lifetime() {}

void ParticleUpdater_2d_Lifetime::update(Particle_2d& particle)
{
    particle.m_lifetime -= GlobalVars::getInstance().timer.getDeltaTime();
}

ParticleUpdater_2d_Speed::ParticleUpdater_2d_Speed() {}

void ParticleUpdater_2d_Speed::update(Particle_2d& particle)
{
    float delta_s = static_cast<float>(GlobalVars::getInstance().timer.getDeltaTime()) / 1000.0f;
    particle.m_originPoint.x += particle.m_speed.x * delta_s;
    particle.m_originPoint.y += particle.m_speed.y * delta_s;
}

ParticleUpdater_2d_Force::ParticleUpdater_2d_Force(float x_acc, float y_acc) : m_accX(x_acc), m_accY(y_acc) {}

void ParticleUpdater_2d_Force::update(Particle_2d& particle)
{
    float delta_s = static_cast<float>(GlobalVars::getInstance().timer.getDeltaTime()) / 1000.0f;
    particle.m_speed.x += m_accX * delta_s;
    particle.m_speed.y += m_accY * delta_s;
}

ParticleUpdater_2d_Color::ParticleUpdater_2d_Color(const SDL_Color& beginColor, const SDL_Color& endColor) :
    m_beginColor(beginColor),
    m_endColor(endColor)
{}

void ParticleUpdater_2d_Color::update(Particle_2d& particle)
{
    float begin_rate = static_cast<float>(particle.m_lifetime) / static_cast<float>(particle.m_totalLifetime);
    float end_rate = 1.0f - begin_rate;
    particle.m_color.r = m_beginColor.r * begin_rate + m_endColor.r * end_rate;
    particle.m_color.g = m_beginColor.g * begin_rate + m_endColor.g * end_rate;
    particle.m_color.b = m_beginColor.b * begin_rate + m_endColor.b * end_rate;
    particle.m_color.a = m_beginColor.a * begin_rate + m_endColor.a * end_rate;
}

ParticleUpdater_2d_Rotate::ParticleUpdater_2d_Rotate(float speed_degree)
{
    // 旋转的速度(radian)
    m_speedRadian = speed_degree;
}

void ParticleUpdater_2d_Rotate::update(Particle_2d& particle)
{
    float delta_s = static_cast<float>(GlobalVars::getInstance().timer.getDeltaTime()) / 1000.0f;
    particle.m_degree += m_speedRadian * delta_s;
}

ParticleUpdater_2d_Size::ParticleUpdater_2d_Size(float beginScacle, float endScale) :
    m_beginScale(beginScacle), m_endScale(endScale)
{}

void ParticleUpdater_2d_Size::update(Particle_2d& particle)
{
    float begin_rate = static_cast<float>(particle.m_lifetime) / static_cast<float>(particle.m_totalLifetime);
    float end_rate = 1.0f - begin_rate;
    particle.m_scale = begin_rate * m_beginScale + end_rate * m_endScale;
}