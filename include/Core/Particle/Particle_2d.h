#ifndef PARTICLE_2D_H
#define PARTICLE_2D_H

#include <vector>
#include <memory>
#include <math.h>

#include "SDL.h"

// 粒子类
struct Particle_2d {
    // 渲染所需参数
    SDL_FPoint m_originPoint;   // 粒子位置
    SDL_Color m_color;          // 粒子颜色 RGBA
    float m_scale;              // 粒子大小(缩放倍数)
    float m_radian;             // 旋转的弧度
    // 更新粒子所需参数
    SDL_FPoint m_speed;         // 粒子速度
    int64_t m_lifetime;         // 粒子剩余存活时间 ms
    int64_t m_totalLifetime;    // 粒子总存活时间 ms
    Particle_2d() :
        m_originPoint{ 0.0f, 0.0f },
        m_color{ 255,255,255,255 },
        m_scale(1.0f),
        m_speed{ 0.0f, 0.0f },
        m_radian(0.0f),
        m_lifetime(0),
        m_totalLifetime(0)
    {}
    inline bool alive() { return m_lifetime > 0; }
};

class ParticleEmitter_2d;

// 粒子构建接口
// 提供粒子各种初始值
class ParticleSetter_2d {
public:
    virtual void set(Particle_2d& particle, ParticleEmitter_2d& emitter) = 0;
    virtual ~ParticleSetter_2d() {};
};

// 粒子更新接口
class ParticleUpdater_2d {
public:
    virtual void update(Particle_2d& particle) = 0;
    virtual ~ParticleUpdater_2d() {};
};

// 粒子渲染接口
class ParticleRenderer_2d {
public:
    virtual void render(Particle_2d& particle) = 0;
    virtual ~ParticleRenderer_2d() {};
};

#define MAX_EMIT_MS 78840000000      // 2.5 years

class ParticleEmitter_2d {
public:
    // 发射器属性
    bool m_valid;                           // true->粒子发射器是合法的 会在render中被渲染
    bool m_oneshoot;                        // 一次性
    SDL_FPoint m_center;                    // 粒子发射器中心坐标
    int64_t m_lifetime;                     // 粒子发射器的剩余存活时间
    int64_t m_totalLifetime;                // 粒子发射器总存活时间
protected:
    // 粒子数据
    uint64_t m_maxParticleNum;              // 最大容纳的粒子数
    std::vector<Particle_2d> m_particles;   // 粒子数组
    uint64_t m_lastGenerateIndex;           // 上一次产生粒子的数组下标
    // 粒子发射信息
    uint64_t m_emitTimePassby;              // 粒子发射器距离上次发射经过的时间
    uint64_t m_emitDeltaTime;               // 发射粒子的时间间隔
    // 粒子构建与更新方法
    std::shared_ptr<ParticleSetter_2d> m_setter;
    std::vector<std::shared_ptr<ParticleUpdater_2d>> m_updater;
    std::shared_ptr<ParticleRenderer_2d> m_renderer;
public:
    ParticleEmitter_2d(
        float x, float y,
        uint64_t max_particle,
        int64_t life_time,
        uint64_t emit_delta_time,
        const std::shared_ptr<ParticleSetter_2d>& setter,
        const std::shared_ptr<ParticleUpdater_2d>& updater,
        const std::shared_ptr<ParticleRenderer_2d>& renderer
    );
    ParticleEmitter_2d(
        float x, float y,
        uint64_t max_particle,
        int64_t life_time,
        uint64_t emit_delta_time,
        const std::shared_ptr<ParticleSetter_2d>& setter,
        const std::initializer_list<std::shared_ptr<ParticleUpdater_2d>>& updater,
        const std::shared_ptr<ParticleRenderer_2d>& renderer
    );

    // 重置(初始化)发射器
    void initilize(float x, float y);
    // 发射粒子
    void emit(uint32_t num = 1);
    // 更新粒子发射器
    void update();
    // 渲染粒子发射器
    void render();

    inline bool valid() { return m_valid; }
    inline void setPosition(float x, float y) { m_center = SDL_FPoint{ x, y }; }
    inline void setOneShoot(bool flag) { m_oneshoot = flag; }

    ~ParticleEmitter_2d();
};
#endif