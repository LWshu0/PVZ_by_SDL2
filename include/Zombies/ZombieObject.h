#ifndef ZOMBIEOBJECT_H
#define ZOMBIEOBJECT_H

#include <memory>

#include "Core/GameObject.h"
#include "Resource/AnimPlayer.h"

enum ZombieType {
    ZombieNormal,
    MaxZombieType
};

enum ZombieState {
    Zombie_IDLE,
    Zombie_WALK,
    Zombie_ATTACK,
    Zombie_DEAD,
    Zombie_ASHES,
    Zombie_SQUISH,
    Zombie_DELETE
};

class ZombieObject :public GameObject {
protected:
    int m_HP;
    ZombieState m_state;
    // 僵尸移动动画的参考点, 每次动画循环更改该点一次
    SDL_FPoint m_referenceScreenPoint;

    SDL_FPoint m_offsetAABB;    // 动画播放位置到aabb位置的偏移量
    SDL_FPoint m_offsetShadow;  // 动画播放位置到阴影位置的偏移量

    AnimPlayer m_animPlayer;
    
    SDL_Texture* m_shadow;
    SDL_FRect m_shadowRange;
public:
    ZombieObject(
        std::shared_ptr<AnimLoader> loader,
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
    virtual std::shared_ptr<ZombieObject> clone(const SDL_FPoint& root_point) = 0;

    // 初始化方法
    virtual void initilize(const SDL_FPoint& root_point) = 0;

    virtual ZombieType getType() = 0;
    inline bool zeroHP() { return m_HP <= 0; }
    inline bool isDead() { return m_state == ZombieState::Zombie_DEAD || m_state == ZombieState::Zombie_ASHES || m_state == ZombieState::Zombie_SQUISH; }
    inline bool canDelete() { return m_state == ZombieState::Zombie_DELETE; }

    inline ZombieState getZombieState() { return m_state; }
    // 改变当前僵尸的状态(ZombieState)
    // 发生改变才会执行 否则直接返回
    virtual int setZombieState(ZombieState to_state) = 0;

    /**
     *@brief 根据计时器中的时间, 计算这段时间内僵尸对植物造成的伤害
     * @param timer 游戏时钟
     * @return int 对植物的伤害值
     */
    virtual int attack() = 0;
    
    // 对僵尸造成伤害, 减少 HP
    // 如果僵尸死亡将切换到死亡动画(默认)
    virtual int damage(int damege_num);

    virtual int update() override;

    // 显示碰撞箱
    int showAABB();
    // 显示阴影
    int showShadow();

    virtual ~ZombieObject();
protected:
    virtual void onUpdateIdle();
    virtual void onUpdateWalk();
    virtual void onUpdateAttack();
    virtual void onUpdateDead();
    virtual void onUpdateAshes();
    virtual void onUpdateSquish();
    virtual void onUpdateDelete();
};

#endif