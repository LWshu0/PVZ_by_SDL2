#include "Zombies/Zombie.h"
/*
0: anim_superlongdeath -> range: [292, 429] total: [0, 503]
1: anim_swim -> range: [250, 292] total: [0, 503]
2: anim_death2 -> range: [217, 250] total: [0, 503]
3: anim_death -> range: [178, 217] total: [0, 503]
4: anim_walk2 -> range: [91, 138] total: [0, 503]
5: anim_walk -> range: [44, 91] total: [0, 503]
6: anim_idle2 -> range: [29, 44] total: [0, 503]
7: anim_idle -> range: [0, 503] total: [0, 503]
8: anim_eat -> range: [138, 178] total: [0, 503]
9: anim_waterdeath -> range: [429, 454] total: [0, 503]
10: anim_dance -> range: [454, 503] total: [0, 503]
11: _ground -> range: [44, 429] total: [0, 503]
12: anim_innerarm3 -> range: [217, 429] total: [0, 503]
13: anim_innerarm2 -> range: [217, 429] total: [0, 503]
14: anim_innerarm1 -> range: [217, 451] total: [0, 503]
15: Zombie_flaghand -> range: [250, 292] total: [0, 503]
16: Zombie_innerarm_screendoor -> range: [250, 292] total: [0, 503]
17: Zombie_neck -> range: [454, 503] total: [0, 503]
18: anim_head1 -> range: [250, 292] total: [0, 503]
19: Zombie_innerleg_upper -> range: [292, 429] total: [0, 503]
20: Zombie_innerleg_lower -> range: [292, 429] total: [0, 503]
21: Zombie_innerleg_foot -> range: [292, 429] total: [0, 503]
22: Zombie_outerleg_upper -> range: [292, 429] total: [0, 503]
23: Zombie_outerleg_foot -> range: [292, 429] total: [0, 503]
24: Zombie_outerleg_lower -> range: [292, 429] total: [0, 503]
25: Zombie_body -> range: [454, 503] total: [0, 503]
26: Zombie_duckytube -> range: [250, 451] total: [0, 503]
27: Zombie_whitewater -> range: [250, 292] total: [0, 503]
28: Zombie_tie -> range: [454, 503] total: [0, 503]
29: anim_head2 -> range: [250, 292] total: [0, 503]
30: anim_tongue -> range: [250, 292] total: [0, 503]
31: Zombie_mustache -> range: [250, 292] total: [0, 503]
32: anim_screendoor -> range: [0, 503] total: [0, 503]
33: Zombie_innerarm_screendoor_hand -> range: [0, 503] total: [0, 503]
34: Zombie_outerarm_screendoor -> range: [0, 503] total: [0, 503]
35: Zombie_outerarm_hand -> range: [454, 503] total: [0, 503]
36: Zombie_outerarm_upper -> range: [454, 503] total: [0, 503]
37: Zombie_whitewater2 -> range: [429, 454] total: [0, 503]
38: Zombie_outerarm_lower -> range: [454, 503] total: [0, 503]
39: anim_hair -> range: [250, 292] total: [0, 503]
40: anim_cone -> range: [250, 292] total: [0, 503]
41: anim_bucket -> range: [250, 292] total: [0, 503]
*/

Zombie::Zombie(
    std::shared_ptr<AnimLoader> loader,
    const SDL_FPoint& root_point
) :
    ZombieObject(
        loader,                                              // 资源
        SDL_FPoint{ root_point.x - 40, root_point.y - 120 }, // 动画播放位置
        SDL_FPoint{ 20, 20 }, 40, 100,                       // 碰撞箱
        SDL_FPoint{ 10, 100 }, 80, 30,                       // 阴影
        200,                                                 // HP
        ZombieState::Zombie_IDLE
    )
{
    // 初始化播放的轨道
    setPlayingTrack(
        { 12, 13, 14, 17, 18, 19, 20, 21, 22, 23, 24, 25, 29, 35, 36, 38 },
        { 6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6 }

    );
    restartTrack(
        { 12, 13, 14, 17, 18, 19, 20, 21, 22, 23, 24, 25, 29, 35, 36, 38 }
    );
}

std::shared_ptr<ZombieObject> Zombie::cloneZombie(const SDL_FPoint& root_point)
{
    return std::make_shared<Zombie>(m_loader, root_point);
}

int Zombie::render()
{
    // 渲染帧
    // 阴影
    showShadow();

    renderTracks({ 12, 13, 14, 17, 18, 19, 20, 21, 22, 23, 24, 25, 29, 35, 36, 38 });

    showAABB();

    return 0;
}

int Zombie::changeAnimState(AnimState to_state)
{
    if (to_state == m_playingAnimState) return 0;
    m_playingAnimState = to_state;
    if (AnimState::R_ATTACK == m_playingAnimState)
    {
        setPlayingTrack(
            { 12, 13, 14, 17, 18, 19, 20, 21, 22, 23, 24, 25, 29, 35, 36, 38 },
            { 8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8 }
        );
        // setFPS(
        //     { 13, 14, 15, 16 },
        //     48.0f
        // );
        restartTrack(
            { 12, 13, 14, 17, 18, 19, 20, 21, 22, 23, 24, 25, 29, 35, 36, 38 }
        );
    }
    else if (AnimState::R_WALK == m_playingAnimState)
    {
        // 4 or 5
        setPlayingTrack(
            { 12, 13, 14, 17, 18, 19, 20, 21, 22, 23, 24, 25, 29, 35, 36, 38, 11 },
            { 5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5 }
        );
        // alignTrack({ 12, 13, 14, 17, 18, 19, 20, 21, 22, 23, 24, 25, 29, 35, 36, 38 }, 11);
        setFPS(
            {12, 13, 14, 17, 18, 19, 20, 21, 22, 23, 24, 25, 29, 35, 36, 38, 11},
            14.0f
        );
        restartTrack(
            { 11, 12, 13, 14, 17, 18, 19, 20, 21, 22, 23, 24, 25, 29, 35, 36, 38 }
        );
        m_referenceScreenPoint.x = m_realtimeScreenPoint.x - 9.8f;
    }
    else    // IDLE
    {
        // 6 or 7
        setPlayingTrack(
            { 12, 13, 14, 17, 18, 19, 20, 21, 22, 23, 24, 25, 29, 35, 36, 38 },
            { 6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6 }

        );
        restartTrack(
            { 12, 13, 14, 17, 18, 19, 20, 21, 22, 23, 24, 25, 29, 35, 36, 38 }
        );
    }
    return 0;
}

int Zombie::changeZombieState(ZombieState to_state)
{
    if (to_state == m_state) return -1;
    m_state = to_state;
    if (ZombieState::Zombie_ATTACK == to_state)
    {
        changeAnimState(AnimState::R_ATTACK);
    }
    else if (ZombieState::Zombie_WALK == to_state)
    {
        changeAnimState(AnimState::R_WALK);
    }
    else
    {
        changeAnimState(AnimState::R_IDLE);
    }
    return 0;
}

int Zombie::attack()
{
    return 1;
}

int Zombie::updateZombie()
{
    // 更新帧
    updatePlayingFrameIdx(GlobalVars::getInstance().timer.getTime());
    if (isUpdateAt(11, GlobalVars::getInstance().timer.getTime()))
    {
        if (isPlayBegin(11))
        {
            m_referenceScreenPoint.x -= 49.8f;
        }
        int playing_ground_frame = m_trackPlayRecord[11].m_playingFrameIdx;
        m_realtimeScreenPoint.x = m_referenceScreenPoint.x - m_loader->m_tracks[11].m_frames[playing_ground_frame].m_x;
        m_aabb.x = m_realtimeScreenPoint.x + m_offsetAABB.x;
        m_shadowRange.x = m_realtimeScreenPoint.x + m_offsetShadow.x;
    }
    return 0;
}

Zombie::~Zombie()
{}