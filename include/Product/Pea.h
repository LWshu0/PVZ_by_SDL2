#ifndef PEA_H
#define PEA_H

#include "BulletObject.h"
#include "Core/Particle/Particle_2d.h"
#include "Core/ObjectPool.h"

class Pea :public BulletObject {
protected:
    SDL_Texture* m_texture;
    int m_textureWidth;
    int m_textureHeight;
    // 粒子发射器
    // 碎裂效果
    std::shared_ptr<ParticleEmitter_2d> m_centerEmitter;
    std::shared_ptr<ParticleEmitter_2d> m_otherEmitter;
public:
    Pea(
        float x,
        float y,
        std::shared_ptr<ParticleEmitter_2d> centerEmitter,
        std::shared_ptr<ParticleEmitter_2d> otherEmitter
    );
    virtual void initilize(float x, float y) override;
    virtual ProductType getType() override;
    virtual void setState(ProductState state) override;
    virtual int update() override;
    virtual int render() override;
    virtual ~Pea() {};
};

class PeaFactory :public ObjectFactory<ProductObject> {
private:
    // 发射器共用参数
    std::shared_ptr<ParticleSetter_2d> setter_center;
    std::shared_ptr<ParticleSetter_2d> setter_other;
    std::shared_ptr<ParticleUpdater_2d> updater_lifetime;
    std::shared_ptr<ParticleUpdater_2d> updater_speed;
    std::shared_ptr<ParticleUpdater_2d> updater_size;
    std::shared_ptr<ParticleUpdater_2d> updater_force;
    std::shared_ptr<ParticleRenderer_2d> render_center;
    std::shared_ptr<ParticleRenderer_2d> render_other;

public:
    PeaFactory();
    virtual std::shared_ptr<ProductObject> create() override;
};

#endif