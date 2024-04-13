#ifndef OBJ_PLANT_H
#define OBJ_PLANT_H

#include "Core/Timer.h"
#include "Core/GameObject.h"
#include "Core/AnimPlayer.h"
#include "Bullet/BulletObject.h"

#include <memory>

enum PlantState {
    IDLE,       // IDLE 状态播放 IDLE 动画
    ATTACK      // 在攻击逻辑时播放攻击动画, 攻击动画间隔用 IDLE 动画填充
};

class PlantObject : public GameObject, public AnimPlayer {
public:
    // 
    int m_HP;
    /*  in the timeline shown below - 代表 IDLE 动画
                                    * 代表 ATTACK 动画
                                    + 代表进入攻击逻辑(等待一段前摇后产生子弹)
        | anim
        ----------*********----------*********----------*********
                  +    |             +                  +
        | logic   | 01 |02|   03     |
    */
    PlantState m_state;                     // 植物当前的状态
    uint64_t m_reloadMilliSecond;           // 植物属性, 固定值, 两次攻击逻辑之间的间隔 即 01+02+03
    uint64_t m_windUpDuration;              // 攻击前摇 即 01

    uint64_t m_nextAttackAnimMilliSecond;   // 下一次 attack 动画播放时间
    uint64_t m_nextFireMilliSecond;         // 下一次 Fire 时间

    SDL_Texture* m_shadow;
    SDL_FRect m_shadowRange;
public:
    PlantObject(
        std::shared_ptr<AnimLoader> loader,
        std::shared_ptr<Camera> camera,
        const SDL_FPoint& init_point,
        int HP,
        uint64_t reload_ms,
        const SDL_FRect& aabb
    );

    PlantObject(
        std::shared_ptr<AnimLoader> loader,
        std::shared_ptr<Camera> camera,
        const SDL_FPoint& init_point,
        int HP,
        int reload_ms,
        const SDL_FPoint& root_point,
        float width,
        float height
    );

    virtual std::shared_ptr<PlantObject> clonePlant(const SDL_FPoint& root_point) = 0;

    // virtual int Play(uint64_t now_ms);
    // virtual int changeDamageState(DamageState to_state);
    // virtual int changeAnimState(AnimState to_state);
    // virtual SDL_FPoint GetPosition();
    // virtual void setPlayPosition(const SDL_FPoint& point);

    inline bool isDead() { return m_HP <= 0; }

    
    // 待实现
    // virtual bool inAttackRange(const SDL_FRect& enemy_aabb) = 0;

    // 改变当前帧状态
    // 发生改变才会执行 否则直接返回
    virtual int changePlantState(PlantState to_state, std::shared_ptr<Timer> timer) = 0;

    // 返回产生的子弹类型, BulletType::MaxBulletType 代表该时刻不产生子弹
    virtual BulletType attack(std::shared_ptr<Timer> timer) = 0;

    virtual int damage(int damege);

    // 更新到下一帧的状态
    virtual int updatePlant(std::shared_ptr<Timer> timer) = 0;

    //
    // virtual int update(uint64_t now_ms) override;
    int showAABB();

    int showShadow();

    virtual ~PlantObject() {};
};

#endif