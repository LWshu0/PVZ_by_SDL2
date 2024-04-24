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
        loader, camera,                                      // 资源 相机   
        SDL_FPoint{ root_point.x - 40, root_point.y - 80 }, // 动画播放位置
        SDL_FPoint{ 20, 0 }, 40, 80,                       // 碰撞箱
        SDL_FPoint{ 0, 55 }, 80, 30,                       // 阴影
        1000,                                                // HP
        PlantState::IDLE,                                    // state
        3000,                                                // reload 时间
        (70 - 54) * 1000 / 48.0f                             // windup duration
    ),
    is_blinking(false),
    last_blink_ms(0),
    delta_blink_ms(5000)
{
    // float default_attack_fps = 48.0f;
    // uint64_t attack_logic_ms = (79 - 54) * 1000 / default_attack_fps;
    // if (attack_logic_ms > m_reloadMilliSecond)
    // {
    //     default_attack_fps = (79 - 54) * 1000 / m_reloadMilliSecond;
    //     attack_logic_ms = m_reloadMilliSecond;
    // }
    // 初始播放的轨道
    setPlayingTrack(
        { 4, 5, 6, 7, 8, 9, 10, 11, 13, 14, 15, 16, 17 },
        { 0, 0, 0, 0, 0, 0, 0,  0,  1,  1,  1,  2,  7 }
    );
    setFPS(
        { 4, 5, 6, 7, 8, 9, 10, 11, 13, 14, 15, 16, 17 },
        14.0f
    );
    restartTrack(
        { 4, 5, 6, 7, 8, 9, 10, 11, 13, 14, 15, 16, 17 }
    );
}

std::shared_ptr<PlantObject> PeaShooterSingle::clonePlant(const SDL_FPoint& root_point)
{
    return std::make_shared<PeaShooterSingle>(m_loader, m_camera, root_point);
}

void PeaShooterSingle::setRootPoint(const SDL_FPoint& root_point)
{
    setRealPoint(SDL_FPoint{ root_point.x - 40, root_point.y - 80 });
}

int PeaShooterSingle::render()
{
    // 渲染帧
    // 阴影
    showShadow();
    // 茎 & 叶
    renderTracks({ 4, 5, 6, 7, 8, 9, 10, 11 });
    // 头
    renderTracks({ 13, 14, 15 }, getOffset(14));

    if (AnimState::R_ATTACK != m_playingAnimState && is_blinking)
    {
        renderTrack(17, getOffset(17));
        if (isPlayEnd(17)) is_blinking = false;
    }
    if (AnimState::R_ATTACK == m_playingAnimState)
    {
        renderTrack(16, getOffset(16));
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
        restartTrack(
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
        restartTrack(
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
        // SDL_Log("change to attack plant\n");
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
    // 更新帧
    updatePlayingFrameIdx(timer->getTime());
    // 眨眼
    if (last_blink_ms + delta_blink_ms < timer->getTime())
    {
        is_blinking = true;
        last_blink_ms = timer->getTime();
    }
    // 攻击动画
    if (PlantState::ATTACK == m_state)
    {
        if (timer->getTime() >= m_nextAttackAnimMilliSecond)
        {
            changeAnimState(AnimState::R_ATTACK);
            // m_nextFireMilliSecond = m_nextAttackAnimMilliSecond + m_windUpDuration;
            m_nextAttackAnimMilliSecond = m_nextAttackAnimMilliSecond + m_reloadMilliSecond;
        }
    }
    return 0;
}

int PeaShooterSingle::changeToStatic()
{
    changeAnimState(AnimState::R_IDLE);
    restartTrack();
    setPlayPosition(SDL_FPoint{ -8.0f, -10.0f });
    return 0;
}

int PeaShooterSingle::getAnimRange(float& width, float& height)
{
    width = 70.0f;
    height = 70.0f;
    return 0;
}

int PeaShooterSingle::renderToTexture()
{
    // body
    renderTracks({ 4, 5, 6, 7, 8, 9, 10, 11 });
    // 头
    renderTracks({ 13, 14, 15 }, getOffset(14));
    return 0;
}

int PeaShooterSingle::renderAlpha()
{
    // body
    renderTracks({ 4, 5, 6, 7, 8, 9, 10, 11 }, SDL_FPoint{ 0.0f, 0.0f }, 200);
    // 头
    renderTracks({ 13, 14, 15 }, getOffset(14), 200);
    return 0;
}

PeaShooterSingle::~PeaShooterSingle()
{

}
