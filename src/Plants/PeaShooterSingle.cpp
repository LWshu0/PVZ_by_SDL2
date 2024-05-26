#include "Plants/PeaShooterSingle.h"
#include "Core/GlobalVars.h"
#include "Manager/ProductManager.h"
#include "Manager/ZombieManager.h"
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
    const SDL_FPoint& root_point
) :
    PlantObject(
        loader,                                                 // 资源  
        SDL_FPoint{ root_point.x - 40, root_point.y - 80 },     // 动画播放位置
        SDL_FPoint{ 20, 0 }, 40, 80,                            // 碰撞箱
        SDL_FPoint{ 0, 55 }, 80, 30,                            // 阴影
        1000,                                                   // HP
        PlantState::Plant_IDLE,                                 // state
        3000,                                                   // reload 时间
        (70 - 54) * 1000 / 48.0f                                // windup duration(第54帧攻击动画开始, 第70帧执行攻击逻辑, 攻击动画帧率为 48 fps)
    ),
    is_blinking(false),
    last_blink_ms(0),
    delta_blink_ms(5000)
{
    // 初始播放的轨道
    m_animPlayer.setPlayingTrack(
        { 4, 5, 6, 7, 8, 9, 10, 11, 13, 14, 15, 16, 17 },
        { 0, 0, 0, 0, 0, 0, 0,  0,  1,  1,  1,  2,  7 }
    );
    m_animPlayer.setFPS(
        { 4, 5, 6, 7, 8, 9, 10, 11, 13, 14, 15, 16, 17 },
        14.0f
    );
    m_animPlayer.restartTrack(
        { 4, 5, 6, 7, 8, 9, 10, 11, 13, 14, 15, 16, 17 }
    );
}

std::shared_ptr<PlantObject> PeaShooterSingle::clone(const SDL_FPoint& root_point)
{
    return std::make_shared<PeaShooterSingle>(m_animPlayer.getAnimLoader(), root_point);
}

void PeaShooterSingle::setRootPoint(const SDL_FPoint& root_point)
{
    setObjectPosition(root_point - SDL_FPoint{ 40.0f, 80.0f });
}

int PeaShooterSingle::render()
{
    // 阴影
    showShadow();
    // 茎 & 叶
    m_animPlayer.renderTracks({ 4, 5, 6, 7, 8, 9, 10, 11 });
    // 头
    m_animPlayer.renderTrackGroup({ 13, 14, 15 }, 14);
    // 眼睛
    if (PlantState::Plant_ATTACK != m_state && is_blinking)
    {
        m_animPlayer.renderTrack(17);
        if (m_animPlayer.isPlayEnd(17)) is_blinking = false;
    }
    // 攻击间隔的 IDLE 动画
    if (PlantState::Plant_ATTACK == m_state)
    {
        m_animPlayer.renderTrack(16);
        if (m_animPlayer.isPlayEnd(15)) setPlantState(PlantState::Plant_IDLE);
    }
    // AABB 碰撞箱
    showAABB();
    return 0;
}

int PeaShooterSingle::setPlantState(PlantState to_state)
{
    if (to_state == m_state) return -1;
    if (PlantState::Plant_ATTACK == to_state)
    {
        // 进入攻击状态前进行时间检查 是否可以进入攻击状态
        if (m_nextAttackAnimMilliSecond > GlobalVars::getInstance().timer.getTime()) return -1;
        // 进入成功 设置为攻击状态
        m_nextAttackAnimMilliSecond = GlobalVars::getInstance().timer.getTime() + m_reloadMilliSecond;
        m_nextFireMilliSecond = GlobalVars::getInstance().timer.getTime() + m_windUpDuration;
        // 设置攻击动画
        m_animPlayer.setPlayingTrack(
            { 4, 5, 6, 7, 8, 9, 10, 11, 13, 14, 15, 16, 17 },
            { 0, 0, 0, 0, 0, 0, 0,  0,  2,  2,  2,  2,  7 }
        );
        m_animPlayer.setFPS(
            { 13, 14, 15, 16 },
            48.0f
        );
        m_animPlayer.restartTrack(
            { 13, 14, 15, 16 }
        );
    }
    else    // 进入 IDLE 状态动画
    {
        m_animPlayer.setPlayingTrack(
            { 4, 5, 6, 7, 8, 9, 10, 11, 13, 14, 15, 16, 17 },
            { 0, 0, 0, 0, 0, 0, 0,  0,  1,  1,  1,  2,  7 }
        );
        m_animPlayer.setFPS(
            { 13, 14, 15, 16 },
            14.0f
        );
        m_animPlayer.restartTrack(
            { 13, 14, 15, 16 }
        );
    }
    m_state = to_state;
    return 0;
}

bool PeaShooterSingle::inAttackRange(const SDL_FRect& enemy_aabb)
{
    return enemy_aabb.x >= m_aabb.x;
}

int PeaShooterSingle::update()
{
    // 更新帧
    m_animPlayer.updatePlayingFrameIdx();
    // 眨眼
    if (last_blink_ms + delta_blink_ms < GlobalVars::getInstance().timer.getTime())
    {
        is_blinking = true;
        last_blink_ms = GlobalVars::getInstance().timer.getTime();
    }
    // 敌人检测
    if (GlobalVars::getInstance().zombieManager->hasZombieInAttackRange(this))
    {
        setPlantState(PlantState::Plant_ATTACK);
    }
    else
    {
        setPlantState(PlantState::Plant_IDLE);
    }
    // 攻击
    if (PlantState::Plant_ATTACK == m_state
        && GlobalVars::getInstance().timer.getTime() >= m_nextFireMilliSecond)
    {
        m_nextFireMilliSecond = m_nextFireMilliSecond + m_reloadMilliSecond;    // 防止在攻击动画播放完成前多次返回子弹
        GlobalVars::getInstance().productManager->addBullet(ProductType::PeaType, m_aabb.x + m_aabb.w, m_aabb.y);
    }
    return 0;
}

int PeaShooterSingle::changeToStatic()
{
    m_animPlayer.setPlayingTrack(
        { 4, 5, 6, 7, 8, 9, 10, 11, 13, 14, 15, 16, 17 },
        { 0, 0, 0, 0, 0, 0, 0,  0,  1,  1,  1,  2,  7 }
    );
    m_animPlayer.setFPS(
        { 13, 14, 15, 16 },
        14.0f
    );
    m_animPlayer.restartTrack(
        { 13, 14, 15, 16 }
    );
    m_animPlayer.restartTrack();
    m_animPlayer.setPlayPosition(SDL_FPoint{ -8.0f, -10.0f });
    return 0;
}

int PeaShooterSingle::getAnimRange(float& width, float& height)
{
    width = 70.0f;
    height = 70.0f;
    return 0;
}

int PeaShooterSingle::renderStatic(uint8_t alpha)
{
    // body
    m_animPlayer.renderTracks({ 4, 5, 6, 7, 8, 9, 10, 11 }, alpha);
    // 头
    m_animPlayer.renderTracks({ 13, 14, 15 }, alpha);
    return 0;
}

PeaShooterSingle::~PeaShooterSingle()
{

}
