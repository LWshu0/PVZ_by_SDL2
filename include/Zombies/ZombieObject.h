#ifndef ZOMBIEOBJECT_H
#define ZOMBIEOBJECT_H

#include <memory>

#include "Core/Timer.h"
#include "Core/GameObject.h"
#include "Core/AnimPlayer.h"

enum ZombieType {
    ZombieNormal,
    MaxZombieType
};

enum ZombieState {
    Zombie_IDLE,
    Zombie_WALK,
    Zombie_ATTACK
};

class ZombieObject :public GameObject, public AnimPlayer {
public:
    int m_HP;
    ZombieState m_state;

    SDL_FPoint m_offsetAABB;    // 动画播放位置到aabb位置的偏移量
    SDL_FPoint m_offsetShadow;  // 动画播放位置到阴影位置的偏移量

    SDL_Texture* m_shadow;
    SDL_FRect m_shadowRange;
public:
    ZombieObject(
        std::shared_ptr<AnimLoader> loader,
        std::shared_ptr<Camera> camera,
        const SDL_FPoint& init_point,
        const SDL_FRect& aabb,
        const SDL_FPoint& offset_shadow,
        float shadow_width,
        float shadow_height,
        int HP,
        ZombieState state
    );

    ZombieObject(
        std::shared_ptr<AnimLoader> loader, // 动画资源
        std::shared_ptr<Camera> camera,     // 相机
        const SDL_FPoint& init_point,       // 动画播放位置, 后续的偏移以此为参照
        const SDL_FPoint& offset_abbb,      // aabb 偏移
        float aabb_width,                   // 碰撞箱宽度
        float aabb_height,                  // 碰撞箱高度
        const SDL_FPoint& offset_shadow,    // 阴影偏移
        float shadow_width,                 // 阴影宽度
        float shadow_height,                // 阴影高度
        int HP,                             // HP
        ZombieState state                   // 僵尸状态
    );

    /**
     *@brief 根据已有的对象构造一个新的对象, 新对象仅位置与原对象不同
     *
     * @param root_point 僵尸的脚部坐标, 该位置用于对齐到地图上僵尸的行走的地面
     * @return std::shared_ptr<PlantObject> 构造的新对象
     */
    virtual std::shared_ptr<ZombieObject> cloneZombie(const SDL_FPoint& root_point) = 0;

    inline bool isDead() { return m_HP <= 0; }

    // 改变当前僵尸的状态(ZombieState)
    // 发生改变才会执行 否则直接返回
    virtual int changeZombieState(ZombieState to_state, std::shared_ptr<Timer> timer) = 0;

    /**
     *@brief 根据计时器中的时间, 计算这段时间内僵尸对植物造成的伤害
     * @param timer 游戏时钟
     * @return int 对植物的伤害值
     */
    virtual int attack(std::shared_ptr<Timer> timer) = 0;

    // 对僵尸造成伤害, 减少 HP
    // 如果僵尸死亡将切换到死亡动画(默认)
    virtual int damage(int damege_num);

    // 更新到下一帧的状态
    virtual int updateZombie(std::shared_ptr<Timer> timer) = 0;

    // 显示碰撞箱
    int showAABB();
    // 显示阴影
    int showShadow();

    ~ZombieObject();
};

#endif