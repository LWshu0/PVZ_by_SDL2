#include "Core/AnimPlayer.h"

AnimPlayer::AnimPlayer(std::shared_ptr<AnimLoader> loader, std::shared_ptr<Camera> camera, const SDL_FPoint& init_point) :
    m_loader(loader),
    m_camera(camera),
    m_realtimeScreenPoint(init_point),
    m_referenceScreenPoint(init_point),
    m_playingAnimState(AnimState::R_IDLE),
    m_damageState(DamageState::R_Damage1)
{
    m_trackPlayRecord.resize(loader->m_tracks.size());
    for (auto& tr : m_trackPlayRecord)
    {
        tr.m_fps = loader->m_fps;
        tr.m_deltaMilliSecond = static_cast<uint64_t>(1000.0f / tr.m_fps);
    }
}

int AnimPlayer::Play(uint64_t now_ms)
{
    updatePlayingFrameIdx(now_ms);
    for (auto idx : m_playingTrack)
    {
        playTrack(idx, now_ms);
    }
    return 0;
}

int AnimPlayer::changeDamageState(DamageState to_state)
{
    m_damageState = to_state;
    return 0;
}

int AnimPlayer::changeAnimState(AnimState to_state)
{
    m_playingAnimState = to_state;
    return 0;
}

SDL_FPoint AnimPlayer::getPlayPosition()
{
    return m_realtimeScreenPoint;
}

void AnimPlayer::setPlayPosition(const SDL_FPoint& point)
{
    m_realtimeScreenPoint = point;
}

int AnimPlayer::updatePlayingFrameIdx(uint64_t now_ms)
{
    for (int track_idx : m_playingTrack)
    {
        if (now_ms >= m_trackPlayRecord[track_idx].m_lastMilliSecond + m_trackPlayRecord[track_idx].m_deltaMilliSecond)
        {
            m_trackPlayRecord[track_idx].m_lastMilliSecond = now_ms;
            m_trackPlayRecord[track_idx].m_playingFrameIdx += 1;
            if (m_trackPlayRecord[track_idx].m_playingFrameIdx > m_trackPlayRecord[track_idx].m_end)
            {
                m_trackPlayRecord[track_idx].m_playingFrameIdx = m_trackPlayRecord[track_idx].m_begin;
            }
        }
    }
    return 0;
}

int AnimPlayer::playTrack(int track_idx)
{
    return m_loader->m_tracks[track_idx].renderTrack(
        m_loader->m_renderer,
        m_camera->getRenderPoint(getRealPoint(track_idx)),
        m_trackPlayRecord[track_idx].m_playingFrameIdx,
        m_trackPlayRecord[track_idx].m_alterTexture,
        m_trackPlayRecord[track_idx].m_maskR,
        m_trackPlayRecord[track_idx].m_maskG,
        m_trackPlayRecord[track_idx].m_maskB
    );
}

int AnimPlayer::playTrack(int track_idx, Uint8 mask_a)
{
    return m_loader->m_tracks[track_idx].renderTrack(
        m_loader->m_renderer,
        m_camera->getRenderPoint(getRealPoint(track_idx)),
        m_trackPlayRecord[track_idx].m_playingFrameIdx,
        mask_a
    );
}

int AnimPlayer::playTracks(const std::initializer_list<int>& track_idx, const SDL_FPoint& offset)
{
    SDL_FPoint real_point{
        m_camera->getRenderX(m_realtimeScreenPoint.x + offset.x),
        m_camera->getRenderY(m_realtimeScreenPoint.y + offset.y)
    };
    for (int idx : track_idx)
    {
        m_loader->m_tracks[idx].renderTrack(
            m_loader->m_renderer,
            real_point,
            m_trackPlayRecord[idx].m_playingFrameIdx,
            m_trackPlayRecord[idx].m_alterTexture,
            m_trackPlayRecord[idx].m_maskR,
            m_trackPlayRecord[idx].m_maskG,
            m_trackPlayRecord[idx].m_maskB
        );
    }
    return 0;
}

int AnimPlayer::playTracks(const std::initializer_list<int>& track_idx, const SDL_FPoint& offset, Uint8 mask_a)
{
    SDL_FPoint real_point{
        m_camera->getRenderX(m_realtimeScreenPoint.x + offset.x),
        m_camera->getRenderY(m_realtimeScreenPoint.y + offset.y)
    };
    for (int idx : track_idx)
    {
        m_loader->m_tracks[idx].renderTrack(
            m_loader->m_renderer,
            real_point,
            m_trackPlayRecord[idx].m_playingFrameIdx,
            m_trackPlayRecord[idx].m_alterTexture,
            mask_a
        );
    }
    return 0;
}

SDL_FPoint AnimPlayer::getRealPoint(int track_idx)
{
    SDL_FPoint screenPointPlusOffset(m_realtimeScreenPoint);
    // 当该轨道当前渲染的帧启用了参照点偏移时
    // 为渲染的位置添加偏移量
    while (m_loader->m_tracks[track_idx].m_frames[m_trackPlayRecord[track_idx].m_playingFrameIdx].m_enableRefer)
    {
        // 该轨道当前渲染的帧下标
        int frame_idx = m_trackPlayRecord[track_idx].m_playingFrameIdx;
        // 该轨道的锚点 将锚点移动到参考点上, 使两者重合
        SDL_FPoint anchor_point = m_loader->m_tracks[track_idx].m_frames[frame_idx].m_anchorPoint;
        // 参照轨道的轨道下标
        //正在渲染的帧下标
        //参考点在锚点数组中的下标
        int refer_track_idx = m_loader->m_tracks[track_idx].m_referTrackIdx;
        int refer_frame_idx = m_trackPlayRecord[refer_track_idx].m_playingFrameIdx;
        int refer_point_idx = m_loader->m_tracks[track_idx].m_referPointIdx;
        SDL_FPoint refer_point = m_loader->m_tracks[refer_track_idx].m_frames[refer_frame_idx].m_referPoint[refer_point_idx];
        screenPointPlusOffset.x += (refer_point.x - anchor_point.x);
        screenPointPlusOffset.y += (refer_point.y - anchor_point.y);
        // 被参考轨道可能也参考了其他轨道 递归的检查计算最终的偏移
        track_idx = refer_track_idx;
    }
    return screenPointPlusOffset;
}

SDL_FPoint AnimPlayer::getOffset(int track_idx)
{
    SDL_FPoint offsetPoint{ 0.0f, 0.0f };
    // 当该轨道当前渲染的帧启用了参照点偏移时
    // 为渲染的位置添加偏移量
    while (m_loader->m_tracks[track_idx].m_frames[m_trackPlayRecord[track_idx].m_playingFrameIdx].m_enableRefer)
    {
        // 该轨道当前渲染的帧下标
        int frame_idx = m_trackPlayRecord[track_idx].m_playingFrameIdx;
        // 该轨道的锚点 将锚点移动到参考点上, 使两者重合
        SDL_FPoint anchor_point = m_loader->m_tracks[track_idx].m_frames[frame_idx].m_anchorPoint;
        // 参照轨道的轨道下标
        //正在渲染的帧下标
        //参考点在锚点数组中的下标
        int refer_track_idx = m_loader->m_tracks[track_idx].m_referTrackIdx;
        int refer_frame_idx = m_trackPlayRecord[refer_track_idx].m_playingFrameIdx;
        int refer_point_idx = m_loader->m_tracks[track_idx].m_referPointIdx;
        SDL_FPoint refer_point = m_loader->m_tracks[refer_track_idx].m_frames[refer_frame_idx].m_referPoint[refer_point_idx];
        offsetPoint.x += (refer_point.x - anchor_point.x);
        offsetPoint.y += (refer_point.y - anchor_point.y);
        // 被参考轨道可能也参考了其他轨道 递归的检查计算最终的偏移
        track_idx = refer_track_idx;
    }
    return offsetPoint;
}

int AnimPlayer::setTrackRGB(int track_idx, Uint8 mask_r, Uint8 mask_g, Uint8 mask_b)
{
    m_trackPlayRecord[track_idx].m_maskR = mask_r;
    m_trackPlayRecord[track_idx].m_maskG = mask_g;
    m_trackPlayRecord[track_idx].m_maskB = mask_b;
    return 0;
}

int AnimPlayer::setTrackRGB(const std::initializer_list<int>& track_idx, Uint8 mask_r, Uint8 mask_g, Uint8 mask_b)
{
    for (auto idx : track_idx)
    {
        m_trackPlayRecord[idx].m_maskR = mask_r;
        m_trackPlayRecord[idx].m_maskG = mask_g;
        m_trackPlayRecord[idx].m_maskB = mask_b;
    }
    return 0;
}

int AnimPlayer::setTrackTexture(int track_idx, SDL_Texture* alter_texture)
{
    m_trackPlayRecord[track_idx].m_alterTexture = alter_texture;
    return 0;
}

int AnimPlayer::initPlayingTrack(const std::initializer_list<int>& playing_track_idx, const std::initializer_list<int>& playing_anim_idx)
{
    m_playingTrack.clear();
    for (auto& idx : playing_track_idx)
    {
        m_playingTrack.push_back(idx);
    }

    int i = 0;
    for (auto& idx : playing_anim_idx)
    {
        m_trackPlayRecord[m_playingTrack[i]].m_begin = m_loader->m_animTracks[idx].begin;
        m_trackPlayRecord[m_playingTrack[i]].m_playingFrameIdx = m_loader->m_animTracks[idx].begin;
        m_trackPlayRecord[m_playingTrack[i]].m_end = m_loader->m_animTracks[idx].end;
        i++;
    }

    return 0;
}

int AnimPlayer::setPlayingTrack(const std::initializer_list<int>& playing_track_idx, const std::initializer_list<int>& playing_anim_idx)
{
    m_playingTrack.clear();
    for (auto& idx : playing_track_idx)
    {
        m_playingTrack.push_back(idx);
    }

    int i = 0;
    for (auto& idx : playing_anim_idx)
    {
        m_trackPlayRecord[m_playingTrack[i]].m_begin = m_loader->m_animTracks[idx].begin;
        // m_trackPlayRecord[m_playingTrack[i]].m_playingFrameIdx = m_loader->m_animTracks[idx].begin;
        m_trackPlayRecord[m_playingTrack[i]].m_end = m_loader->m_animTracks[idx].end;
        i++;
    }

    return 0;
}

int AnimPlayer::setFPS(int track_idx, float fps)
{
    if (fps <= 0) return -1;
    m_trackPlayRecord[track_idx].m_fps = fps;
    m_trackPlayRecord[track_idx].m_deltaMilliSecond = static_cast<uint64_t>(1000.0f / fps);
    return 0;
}

int AnimPlayer::setFPS(const std::initializer_list<int>& track_idx, float fps)
{
    if (fps <= 0) return -1;
    for (auto idx : track_idx)
    {
        m_trackPlayRecord[idx].m_fps = fps;
        m_trackPlayRecord[idx].m_deltaMilliSecond = static_cast<uint64_t>(1000.0f / fps);
    }
    return 0;
}
