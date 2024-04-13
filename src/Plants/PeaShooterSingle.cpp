#include "Plants/PeaShooterSingle.h"

/*
    fps: 12
    0 : anim_idle->range : [4, 29] total : [0, 103]
    1 : anim_head_idle->range : [29, 54] total : [0, 103]
    2 : anim_shooting->range : [54, 79] total : [0, 103]
    3 : anim_full_idle->range : [79, 103] total : [0, 103]
    4 : backleaf->range : [4, 29] total : [0, 103]
    5 : backleaf_left_tip->range : [4, 29] total : [0, 103]
    6 : backleaf_right_tip->range : [4, 29] total : [0, 103]
    7 : stalk_bottom->range : [4, 29] total : [0, 103]
    8 : stalk_top->range : [4, 29] total : [0, 103]
    9 : frontleaf->range : [4, 29] total : [0, 103]
    10 : frontleaf_right_tip->range : [4, 29] total : [0, 103]
    11 : frontleaf_tip_left->range : [4, 29] total : [0, 103]
    12 : anim_stem->range : [4, 29] total : [0, 103]
    13 : anim_sprout->range : [29, 103] total : [0, 103]
    14 : anim_face->range : [29, 103] total : [0, 103]
    15 : idle_mouth->range : [29, 103] total : [0, 103]
    16 : idle_shoot_blink->range : [64, 70] total : [0, 103]
    17 : anim_blink->range : [1, 4] total : [0, 103]
*/
PeaShooterSingle::PeaShooterSingle(
    std::shared_ptr<AnimLoader> loader,
    std::shared_ptr<Camera> camera,
    const SDL_FPoint& root_point
) :
    PlantObject(
        loader, camera,                                         // 资源 相机
        SDL_FPoint{ root_point.x - 50, root_point.y - 80 },     // 动画播放位置
        100,                                                    // HP
        3000,                                                   // reload 时间
        root_point, 40, 70                                      // 碰撞箱
    ),
    is_blinking(false),
    last_blink_ms(0),
    delta_blink_ms(5000)
{
    float default_attack_fps = 48.0f;
    // uint64_t attack_logic_ms = (79 - 54) * 1000 / default_attack_fps;
    // if (attack_logic_ms > m_reloadMilliSecond)
    // {
    //     default_attack_fps = (79 - 54) * 1000 / m_reloadMilliSecond;
    //     attack_logic_ms = m_reloadMilliSecond;
    // }
    // 动画时间
    m_windUpDuration = (70 - 54) * 1000 / default_attack_fps;
    // 初始播放的轨道
    initPlayingTrack(
        { 4, 5, 6, 7, 8, 9, 10, 11, 13, 14, 15, 16, 17 },
        { 0, 0, 0, 0, 0, 0, 0,  0,  1,  1,  1,  2,  7 }
    );
    // 阴影
    m_shadowRange = SDL_FRect{ root_point.x - 50, root_point.y - 25, 80, 30 };
}

std::shared_ptr<PlantObject> PeaShooterSingle::clonePlant(const SDL_FPoint& root_point)
{
    return std::make_shared<PeaShooterSingle>(m_loader, m_camera, root_point);
}

int PeaShooterSingle::Play(uint64_t now_ms)
{
    // 更新帧
    updatePlayingFrameIdx(now_ms);
    // 渲染帧
    // 阴影
    showShadow();
    // 茎 & 叶
    playTracks({ 4, 5, 6, 7, 8, 9, 10, 11 });
    // 头
    playTracks({ 13, 14, 15 }, getOffset(14));
    // 眨眼
    if (last_blink_ms + delta_blink_ms < now_ms)
    {
        is_blinking = true;
        last_blink_ms = now_ms;
    }
    if (AnimState::R_ATTACK != m_playingAnimState && is_blinking)
    {
        playTrack(17);
        if (isPlayEnd(17)) is_blinking = false;
    }
    if (AnimState::R_ATTACK == m_playingAnimState)
    {
        playTrack(16);
        if (isPlayEnd(15)) changeAnimState(AnimState::R_IDLE);
    }
    showAABB();
    return 0;
}

int PeaShooterSingle::changeAnimState(AnimState to_state)
{
    if (to_state == m_playingAnimState) return 0;
    m_playingAnimState = to_state;
    if (AnimState::R_ATTACK == m_playingAnimState)
    {
        setPlayingTrack(
            { 4, 5, 6, 7, 8, 9, 10, 11, 13, 14, 15, 16, 17 },
            { 0, 0, 0, 0, 0, 0, 0,  0,  2,  2,  2,  2,  7 }
        );
        setFPS(
            { 13, 14, 15, 16 },
            48.0f
        );
        resetTrack(
            { 13, 14, 15, 16 }
        );
    }
    else
    {
        setPlayingTrack(
            { 4, 5, 6, 7, 8, 9, 10, 11, 13, 14, 15, 16, 17 },
            { 0, 0, 0, 0, 0, 0, 0,  0,  1,  1,  1,  2,  7 }
        );
        setFPS(
            { 13, 14, 15, 16 },
            14.0f
        );
        resetTrack(
            { 13, 14, 15, 16 }
        );
    }
    return 0;
}

int PeaShooterSingle::changePlantState(PlantState to_state, std::shared_ptr<Timer> timer)
{
    if (to_state == m_state) return 0;
    m_state = to_state;
    if (PlantState::ATTACK == to_state)
    {
        m_nextAttackAnimMilliSecond = timer->getTime();
        m_nextFireMilliSecond = timer->getTime() + m_windUpDuration;
        changeAnimState(AnimState::R_ATTACK);
    }
    else
    {
        changeAnimState(AnimState::R_IDLE);
    }
    
    return -1;
}

BulletType PeaShooterSingle::attack(std::shared_ptr<Timer> timer)
{
    if (PlantState::ATTACK != m_state) return BulletType::MaxBulletType;
    if (timer->getTime() >= m_nextFireMilliSecond)
    {
        m_nextFireMilliSecond = m_nextFireMilliSecond + m_reloadMilliSecond;
        return BulletType::BulletPea;
    }
    return BulletType::MaxBulletType;
}

int PeaShooterSingle::updatePlant(std::shared_ptr<Timer> timer)
{
    if (PlantState::ATTACK == m_state)
    {
        if (timer->getTime() >= m_nextAttackAnimMilliSecond)
        {
            changeAnimState(AnimState::R_ATTACK);
            m_nextAttackAnimMilliSecond = m_nextAttackAnimMilliSecond + m_reloadMilliSecond;
        }
    }
    return 0;
}

PeaShooterSingle::~PeaShooterSingle()
{

}
