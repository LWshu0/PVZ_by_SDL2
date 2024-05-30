#ifndef PARTICLERENDERER_2D_H
#define PARTICLERENDERER_2D_H

#include "Particle_2d.h"

// 渲染一张静态矩形贴图到粒子的范围中
class ParticleRenderer_2d_default : public ParticleRenderer_2d
{
protected:
    int m_texWidth, m_texHeight;
    float m_dstRectWidth_half, m_dstRectHeight_half;
    SDL_Texture* m_texture;
public:
    ParticleRenderer_2d_default(SDL_Texture* texture);
    virtual void render(Particle_2d& particle) override;
    virtual ~ParticleRenderer_2d_default() {};
};

// 精灵图播放一张纹理 仅一行
class ParticleRenderer_2d_Sprite : public ParticleRenderer_2d {
protected:
    int m_texWidth, m_texHeight;
    float m_frameWidth;
    float m_dstRectWidth_half, m_dstRectHeight_half;
    SDL_Texture* m_texture;
    uint32_t m_colFrameNum;
public:
    ParticleRenderer_2d_Sprite(SDL_Texture* texture, uint32_t col_num);
    virtual void render(Particle_2d& particle) override;
    virtual ~ParticleRenderer_2d_Sprite() {};
};

#endif