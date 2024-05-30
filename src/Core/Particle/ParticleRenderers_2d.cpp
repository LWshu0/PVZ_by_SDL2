#include "Core/Particle/ParticleRenderers_2d.h"
#include "Core/GlobalVars.h"

ParticleRenderer_2d_default::ParticleRenderer_2d_default(SDL_Texture* texture) :
    ParticleRenderer_2d(),
    m_texture(texture)
{
    SDL_QueryTexture(m_texture, NULL, NULL, &m_texWidth, &m_texHeight);
    m_dstRectWidth_half = m_texWidth / 2;
    m_dstRectHeight_half = m_texHeight / 2;
}

void ParticleRenderer_2d_default::render(Particle_2d& particle)
{
    SDL_FRect rect{
        particle.m_originPoint.x - m_dstRectWidth_half * particle.m_scale,
        particle.m_originPoint.y - m_dstRectHeight_half * particle.m_scale,
        m_dstRectWidth_half * 2 * particle.m_scale,
        m_dstRectHeight_half * 2 * particle.m_scale
    };
    SDL_RenderCopyExF(
        GlobalVars::getInstance().renderer,
        m_texture,
        NULL,
        &rect,
        particle.m_radian,
        NULL,
        SDL_RendererFlip::SDL_FLIP_NONE
    );
}

ParticleRenderer_2d_Sprite::ParticleRenderer_2d_Sprite(SDL_Texture* texture, uint32_t col_num) :
    ParticleRenderer_2d(),
    m_texture(texture),
    m_colFrameNum(col_num)
{
    SDL_QueryTexture(m_texture, NULL, NULL, &m_texWidth, &m_texHeight);
    m_frameWidth = (float)m_texWidth / col_num;
    m_dstRectWidth_half = m_frameWidth / 2;
    m_dstRectHeight_half = m_texHeight / 2;
}

void ParticleRenderer_2d_Sprite::render(Particle_2d& particle)
{
    int frame_idx = (particle.m_totalLifetime - particle.m_lifetime) * m_colFrameNum / particle.m_totalLifetime;
    SDL_Rect src_rect{
        (int)(m_frameWidth * frame_idx),
        0,
        (int)m_frameWidth,
        m_texHeight
    };
    SDL_FRect dst_rect{
        particle.m_originPoint.x - m_dstRectWidth_half * particle.m_scale,
        particle.m_originPoint.y - m_dstRectHeight_half * particle.m_scale,
        m_dstRectWidth_half * 2 * particle.m_scale,
        m_dstRectHeight_half * 2 * particle.m_scale
    };
    SDL_RenderCopyExF(
        GlobalVars::getInstance().renderer,
        m_texture,
        &src_rect,
        &dst_rect,
        particle.m_radian,
        NULL,
        SDL_RendererFlip::SDL_FLIP_NONE
    );
}
