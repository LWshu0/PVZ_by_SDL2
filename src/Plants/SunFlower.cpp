#include "Plants/SunFlower.h"
/*
fps: 12
0: backleaf -> range: [4, 28] total: [0, 28]
1: backleaf_left_tip -> range: [4, 28] total: [0, 28]
2: backleaf_right_tip -> range: [4, 28] total: [0, 28]
3: stalk_bottom -> range: [4, 28] total: [0, 28]
4: stalk_top -> range: [4, 28] total: [0, 28]
5: frontleaf -> range: [4, 28] total: [0, 28]
6: frontleaf_right_tip -> range: [4, 28] total: [0, 28]
7: frontleaf_left_tip -> range: [4, 28] total: [0, 28]
8: SunFlower_leftpetal8 -> range: [4, 28] total: [0, 28]
9: SunFlower_leftpetal7 -> range: [4, 28] total: [0, 28]
10: SunFlower_leftpetal6 -> range: [4, 28] total: [0, 28]
11: SunFlower_leftpetal5 -> range: [4, 28] total: [0, 28]
12: SunFlower_leftpetal4 -> range: [4, 28] total: [0, 28]
13: SunFlower_leftpetal3 -> range: [4, 28] total: [0, 28]
14: SunFlower_leftpetal2 -> range: [4, 28] total: [0, 28]
15: SunFlower_leftpetal1 -> range: [4, 28] total: [0, 28]
16: SunFlower_bottompetals -> range: [4, 28] total: [0, 28]
17: SunFlower_rightpetal9 -> range: [4, 28] total: [0, 28]
18: SunFlower_rightpetal8 -> range: [4, 28] total: [0, 28]
19: SunFlower_rightpetal7 -> range: [4, 28] total: [0, 28]
20: SunFlower_rightpetal6 -> range: [4, 28] total: [0, 28]
21: SunFlower_rightpetal5 -> range: [4, 28] total: [0, 28]
22: SunFlower_rightpetal4 -> range: [4, 28] total: [0, 28]
23: SunFlower_rightpetal3 -> range: [4, 28] total: [0, 28]
24: SunFlower_rightpetal2 -> range: [4, 28] total: [0, 28]
25: SunFlower_rightpetal1 -> range: [4, 28] total: [0, 28]
26: SunFlower_toppetals -> range: [4, 28] total: [0, 28]
27: anim_idle -> range: [4, 28] total: [0, 28]
28: anim_blink -> range: [1, 4] total: [0, 28]
*/

SunFlower::SunFlower(
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
    m_animPlayer.setPlayingTrack(
        { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27 },
        0
    );
    m_animPlayer.setFPS(
        { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27 },
        14.0f
    );
    m_animPlayer.restartTrack();
}

std::shared_ptr<PlantObject> SunFlower::clonePlant(const SDL_FPoint& root_point)
{
    return std::make_shared<SunFlower>(m_animPlayer.getAnimLoader(), root_point);
}

void SunFlower::setRootPoint(const SDL_FPoint& root_point)
{
    setObjectPosition(root_point - SDL_FPoint{ 40.0f, 80.0f });
}

int SunFlower::setPlantState(PlantState to_state)
{
    if (to_state == m_state) return -1;
    // 进入 IDLE 状态动画
    m_animPlayer.setPlayingTrack(
        { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27 },
        0
    );
    m_animPlayer.setFPS(
        { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27 },
        14.0f
    );
    m_animPlayer.restartTrack();
    m_state = PlantState::Plant_IDLE;
    return 0;
}

bool SunFlower::inAttackRange(const SDL_FRect& enemy_aabb)
{
    return false;
}

ProductType SunFlower::attack()
{
    return ProductType::MaxProductNum;
}

int SunFlower::updatePlant()
{
    // 更新帧
    m_animPlayer.updatePlayingFrameIdx();
    // 眨眼
    if (!is_blinking && last_blink_ms + delta_blink_ms < GlobalVars::getInstance().timer.getTime())
    {
        m_animPlayer.pushPlayingTrack(28, 1);
        m_animPlayer.restartTrack(28);
        is_blinking = true;
        last_blink_ms = GlobalVars::getInstance().timer.getTime();
    }
    return 0;
}

int SunFlower::changeToStatic()
{
    m_animPlayer.setPlayingTrack(
        { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27 },
        0
    );
    m_animPlayer.setFPS(
        { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27 },
        14.0f
    );
    m_animPlayer.restartTrack(
        { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27 }
    );
    m_animPlayer.restartTrack();
    m_animPlayer.setPlayPosition(SDL_FPoint{ -8.0f, -10.0f });
    return 0;
}

int SunFlower::getAnimRange(float& width, float& height)
{
    width = 70.0f;
    height = 70.0f;
    return 0;
}

int SunFlower::renderStatic(uint8_t alpha)
{
    m_animPlayer.render();
    return 0;
}

int SunFlower::render()
{
    // 阴影
    showShadow();
    // 茎 & 叶
    m_animPlayer.render();
    // 眼睛
    if (is_blinking)
    {
        m_animPlayer.renderTrack(28);
        if (m_animPlayer.isPlayEnd(28))
        {
            is_blinking = false;
            m_animPlayer.popPlayingTrack();
        }
    }
    // AABB 碰撞箱
    showAABB();
    return 0;
}

SunFlower::~SunFlower()
{}