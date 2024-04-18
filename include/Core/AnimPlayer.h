#ifndef ANIMPLAYER_H
#define ANIMPLAYER_H

#include <vector>
#include <memory>
#include <cmath>
#include <string>
#include "SDL.h"

#include "AnimLoader.h"
#include "Camera.h"

enum AnimState {
    R_IDLE,     /*  空闲状态, 植物不发射, 僵尸不行走
                *   阳光, 弹幕飞行
                *   界面空闲阶段*/
    R_ATTACK,   /*  攻击状态, 植物发射子弹 / 爆炸, 僵尸啃食 / 其他攻击方式
                *   弹幕与僵尸碰撞后碎裂*/
    R_SLEEP,    // 植物睡眠状态
    R_WALK,     // 僵尸行走状态
    R_DEAD,     // 僵尸死亡过程
    R_SWIM,     // 僵尸游泳

    // 多阶段界面动画播放

    R_ANIM1,    /*进入主界面
                * 僵尸手 出现*/
    R_ANIM2,    /*主界面选择冒险
                * 僵尸手静止*/
    R_ANIM3,    /*主界面选择小游戏*/
    R_ANIM4,    /*主界面选择解密*/
    R_ANIM5     /*主界面选择生存*/
};

// 受损程度
enum DamageState {
    R_Damage1,  // 最完好状态
    R_Damage2,  // 
    R_Damage3,  //
    R_Damage4,  //
    R_Damage5,  // 最损坏状态
    R_Death     // 需要销毁对象
};

// 动画播放器
// 后根据不同的物体做不同的初始化
class AnimPlayer {
protected:
    // 动画数据
    std::shared_ptr<AnimLoader> m_loader;
    // 相机
    std::shared_ptr<Camera> m_camera;
    // 动画播放状态

    // 物体显示的屏幕坐标原点
    // 僵尸的移动是由动画定义的, 需要在动画更新时设置该位置
    // 植物是静止的, 该值等于 init_screen_point
    SDL_FPoint m_realtimeScreenPoint;
    // 僵尸移动动画的参考点, 每次动画循环更改该点一次
    // 植物并不需要该点
    SDL_FPoint m_referenceScreenPoint;

    // 动画播放状态
    AnimState m_playingAnimState;
    // 物体受损程度
    DamageState m_damageState;
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
    // 需要播放的轨道
    std::vector<int> m_playingTrack;
public:
    AnimPlayer(
        std::shared_ptr<AnimLoader> loader,
        std::shared_ptr<Camera> camera,
        const SDL_FPoint& init_point,
        AnimState anim_state = AnimState::R_IDLE,
        DamageState damage_state = DamageState::R_Damage1
    );
    // 动画播放控制

    /*  调用 render 之前需要先 updatePlayingFrameIdx 更新当前需要渲染的帧下标
     *  如果不更新, 动画将停留在起始帧 */
    int updatePlayingFrameIdx(uint64_t now_ms);
    /* 更新帧的同时进行移动(僵尸用) */
    int updatePlayingFrameIdx(uint64_t now_ms, int ground_track_idx, float cycle_offset_x);

    // 渲染指定轨道
    int renderTrack(int track_idx);
    int renderTrack(int track_idx, Uint8 mask_a);
    // 将输入的轨道偏移 offset 渲染
    int renderTracks(const std::initializer_list<int>& track_idx, const SDL_FPoint& offset = SDL_FPoint{ 0.0f, 0.0f });
    int renderTracks(const std::initializer_list<int>& track_idx, const SDL_FPoint& offset, Uint8 mask_a);
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

    int restartTrack(int track_idx);
    int restartTrack(const std::initializer_list<int>& track_idx);

    // 使 track_idx 轨道正在播放的帧对齐到 ref_track_idx 轨道
    // 使两者正在播放的帧下标相同
    int alignTrack(int track_idx, int ref_track_idx);
    int alignTrack(const std::initializer_list<int>& track_idx, int ref_track_idx);

    // 设置 track_idx 号轨道的动画数据
    // 将 track_idx 号轨道按照第 anim_idx 个动画的数据重置到起始帧
    // 同时更改 record 中的起始点和终止点
    int setTrackAnim(int track_idx, int anim_idx);

    int setFPS(int track_idx, float fps);
    int setFPS(const std::initializer_list<int>& track_idx, float fps);

    /**************************************
    *    animation state transition       *
    ***************************************/
    // 受损状态转移
    virtual int changeDamageState(DamageState to_state);
    // 动画状态转移
    virtual int changeAnimState(AnimState to_state);

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
    // 获取轨道当前播放帧的 x 偏移
    // 仅在僵尸移动时, 用于获取 _ground 轨道的 x 数据, 使移动符合实际
    float getPlayingX(int track_idx);
};

#endif