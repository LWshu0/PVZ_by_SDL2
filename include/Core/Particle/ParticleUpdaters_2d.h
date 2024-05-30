#ifndef PARTICLEUPDATERS_2D_H
#define PARTICLEUPDATERS_2D_H

#include "Particle_2d.h"

// 生命值模块 随时间减少生命值
class ParticleUpdater_2d_Lifetime : public ParticleUpdater_2d {
public:
    ParticleUpdater_2d_Lifetime();
    virtual void update(Particle_2d& particle) override;
    virtual ~ParticleUpdater_2d_Lifetime() {};
};

// 速度模块 可以根据当前速度进行移动
class ParticleUpdater_2d_Speed : public ParticleUpdater_2d {
public:
    ParticleUpdater_2d_Speed();
    virtual void update(Particle_2d& particle) override;
    virtual ~ParticleUpdater_2d_Speed() {};
};

// 加速度模块 对粒子的速度进行更改
class ParticleUpdater_2d_Force : public ParticleUpdater_2d {
protected:
    float m_accX, m_accY;
public:
    ParticleUpdater_2d_Force(float x_acc, float y_acc);
    virtual void update(Particle_2d& particle) override;
    virtual ~ParticleUpdater_2d_Force() {};
};

// 颜色变化模块 颜色随粒子生命值逐渐从 begin 变为 end
class ParticleUpdater_2d_Color : public ParticleUpdater_2d {
protected:
    SDL_Color m_beginColor;
    SDL_Color m_endColor;
public:
    // 粒子颜色的终点
    ParticleUpdater_2d_Color(const SDL_Color& beginColor, const SDL_Color& endColor);
    virtual void update(Particle_2d& particle) override;
    virtual ~ParticleUpdater_2d_Color() {};
};

// 旋转模块 根据模块中的角速度绕 origin 进行旋转
class ParticleUpdater_2d_Rotate : public ParticleUpdater_2d {
protected:
    float m_speedRadian;
public:
    // 传入旋转的速度(degree/s)
    ParticleUpdater_2d_Rotate(float speed_degree);
    virtual void update(Particle_2d& particle) override;
    virtual ~ParticleUpdater_2d_Rotate() {};
};

// 缩放模块 以中心为原点进行缩放
class ParticleUpdater_2d_Size : public ParticleUpdater_2d {
protected:
    float m_beginScale;
    float m_endScale;
public:
    // 粒子大小的终点
    ParticleUpdater_2d_Size(float beginScacle, float endScale);
    virtual void update(Particle_2d& particle) override;
    virtual ~ParticleUpdater_2d_Size() {};
};

#endif