#ifndef ANIMPLAYER_H
#define ANIMPLAYER_H

#include <vector>
#include <memory>
#include <cmath>
#include <string>
#include "SDL.h"

#include "Resource/AnimLoader.h"

// 动画播放器
// 后根据不同的物体做不同的初始化
class AnimPlayer {
protected:
    // 动画数据
    std::shared_ptr<AnimLoader> m_loader;

    // 物体显示的屏幕坐标原点
    SDL_FPoint m_realtimeScreenPoint;

    // m_loader 中的每个轨道当前正在播放的状态
    struct TrackPlayRecord {
        uint64_t m_lastMilliSecond = 0;
        uint64_t m_deltaMilliSecond = 0;
        int m_playingFrameIdx = 0;
        float m_fps = 1.0f;
        int m_begin = 0;
        int m_end = 0;
        SDL_Texture* m_alterTexture = nullptr;
        Uint8 m_maskR = 255;
        Uint8 m_maskG = 255;
        Uint8 m_maskB = 255;
    };
    std::vector<AnimPlayer::TrackPlayRecord> m_trackPlayRecord;
    // 需要更新的轨道
    std::vector<int> m_playingTrack;
public:
    AnimPlayer(
        std::shared_ptr<AnimLoader> loader,
        const SDL_FPoint& init_point
    );
    // 动画播放控制

    /*  调用 render 之前需要先 updatePlayingFrameIdx 更新当前需要渲染的帧下标
     *  如果不更新, 动画将停留在起始帧 */
    int updatePlayingFrameIdx();

    // 渲染指定轨道
    int renderTrack(int track_idx, SDL_FPoint ext_offset = SDL_FPoint{ 0.0f, 0.0f }); // 在动画偏移的基础上额外进行一个偏移
    int renderTrack(int track_idx, Uint8 mask_a);

    // 将输入的轨道偏移 offset_ext 渲染
    int renderTracks(const std::initializer_list<int>& track_idx);
    int renderTracks(const std::initializer_list<int>& track_idx, Uint8 mask_a);

    // 将一些轨道视为一组, 按照同一个轨道的参数计算偏移并渲染
    int renderTrackGroup(const std::initializer_list<int>& track_idx, int mainTrack_idx);
    int renderTrackGroup(const std::initializer_list<int>& track_idx, int mainTrack_idx, Uint8 mask_a);

    // 在设定的动画播放位置渲染正在播放的所有轨道
    virtual int render();

    int setTrackRGB(int track_idx, Uint8 mask_r, Uint8 mask_g, Uint8 mask_b);
    int setTrackRGB(const std::initializer_list<int>& track_idx, Uint8 mask_r, Uint8 mask_g, Uint8 mask_b);
    int setTrackTexture(int track_idx, SDL_Texture* alter_texture);

    /**************************************
    *  animation playing process control  *
    ***************************************/
    // 设置播放的动画 (不重置至动画起始位置)
    int setPlayingTrack(const std::initializer_list<int>& playing_track_idx, int playing_anim_idx);
    // 设置播放的动画 (不重置至动画起始位置)
    int setPlayingTrack(const std::initializer_list<int>& playing_track_idx, const std::initializer_list<int>& playing_anim_idx);

    int pushPlayingTrack(int playing_track_idx, int playing_anim_idx);
    int popPlayingTrack();

    int restartTrack();
    int restartTrack(int track_idx);
    int restartTrack(const std::initializer_list<int>& track_idx);

    // 使 track_idx 轨道正在播放的帧对齐到 ref_track_idx 轨道
    // 使两者正在播放的帧下标相同
    int alignTrack(int track_idx, int ref_track_idx);
    int alignTrack(const std::initializer_list<int>& track_idx, int ref_track_idx);

    // 将当前处于播放状态的轨道置于指定的帧位置
    // dst_frame_idx 为逻辑帧下标, 相对于开始位置的偏移
    int gotoFrame(int dst_frame_idx);

    // 设置 track_idx 号轨道的动画数据
    // 将 track_idx 号轨道按照第 anim_idx 个动画的数据重置到起始帧
    // 同时更改 record 中的起始点和终止点
    int setTrackAnim(int track_idx, int anim_idx);

    int setFPS(int track_idx, float fps);
    int setFPS(const std::initializer_list<int>& track_idx, float fps);

    // 获取轨道正在播放的帧
    inline const AnimFrame& getPlayingFrame(int track_idx)
    {
        return m_loader->m_tracks[track_idx].m_frames[m_trackPlayRecord[track_idx].m_playingFrameIdx];
    }
    inline std::shared_ptr<AnimLoader> getAnimLoader() { return m_loader; }
    // 动画播放位置
    inline SDL_FPoint getPlayPosition()
    {
        return m_realtimeScreenPoint;
    }
    inline void setPlayPosition(const SDL_FPoint& point)
    {
        m_realtimeScreenPoint = point;
    }
    // 判断指定轨道是否正在播放第一帧
    inline bool isPlayBegin(int track_idx)
    {
        return m_trackPlayRecord[track_idx].m_playingFrameIdx == m_trackPlayRecord[track_idx].m_begin;
    }
    // 判断播放到结束帧
    inline bool isPlayEnd(int track_idx)
    {
        return m_trackPlayRecord[track_idx].m_playingFrameIdx == m_trackPlayRecord[track_idx].m_end;
    }
    // 判断轨道是否是在当前时刻更新的
    inline bool isUpdateAt(int track_idx, uint64_t now_ms)
    {
        return now_ms == m_trackPlayRecord[track_idx].m_lastMilliSecond;
    }

    virtual ~AnimPlayer() {}

protected:
    // 如果使用了参考点 计算偏移后的真实渲染位置
    // 否则返回默认的渲染位置
    SDL_FPoint getRealPoint(int track_idx);
    // 如果使用了参考点 计算偏移距离
    // 否则返回 {0, 0}
    SDL_FPoint getOffset(int track_idx);
};

#endif