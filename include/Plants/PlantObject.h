#ifndef OBJ_PLANT_H
#define OBJ_PLANT_H

#include "Core/Timer.h"
#include "Core/GameObject.h"
#include "Core/AnimPlayer.h"
#include "Bullet/BulletObject.h"

#include <memory>

enum PlantType {
    PlantPeaShooter1,
    MaxPlantType
};

enum PlantState {
    IDLE,       // IDLE 状态播放 IDLE 动画
    ATTACK      // 在攻击逻辑时播放攻击动画, 攻击动画间隔用 IDLE 动画填充
};

class PlantObject : public GameObject, public AnimPlayer {
public:
    // 植物属性
    int m_HP;
    PlantState m_state;                     // 植物当前的状态
    uint64_t m_reloadMilliSecond;           // 两次攻击逻辑之间的间隔 即下图中 01+02+03
    /*  in the timeline shown below - 代表 IDLE 动画
                                    * 代表 ATTACK 动画
                                    + 代表进入攻击逻辑(等待一段前摇后产生子弹)
        | anim
        ----------*********----------*********----------*********
                  +    |             +                  +
        | logic   | 01 |02|   03     |
        01: 攻击前摇
        02: 攻击后摇
        03: 攻击动画之间的 idle 动画填充
    */
    uint64_t m_windUpDuration;              // 攻击前摇 即 01
    uint64_t m_nextAttackAnimMilliSecond;   // 下一次 attack 动画播放时间
    uint64_t m_nextFireMilliSecond;         // 下一次 Fire 时间

    SDL_FPoint m_offsetAABB;    // 动画播放位置到aabb位置的偏移量
    SDL_FPoint m_offsetShadow;  // 动画播放位置到阴影位置的偏移量

    // 植物影子
    SDL_Texture* m_shadow;
    SDL_FRect m_shadowRange;
public:
    PlantObject(
        std::shared_ptr<AnimLoader> loader,
        std::shared_ptr<Camera> camera,
        const SDL_FPoint& init_point,
        const SDL_FRect& aabb,
        const SDL_FPoint& offset_shadow,
        float shadow_width,
        float shadow_height,
        int HP,
        PlantState state,
        uint64_t reload_ms,
        uint64_t windup_duration
    );

    PlantObject(
        std::shared_ptr<AnimLoader> loader,
        std::shared_ptr<Camera> camera,
        const SDL_FPoint& init_point,
        const SDL_FPoint& offset_abbb,      // aabb 偏移
        float aabb_width,                   // 碰撞箱宽度
        float aabb_height,                  // 碰撞箱高度
        const SDL_FPoint& offset_shadow,    // 阴影偏移
        float shadow_width,                 // 阴影宽度
        float shadow_height,                // 阴影高度
        int HP,
        PlantState state,
        int reload_ms,
        uint64_t windup_duration
    );

    virtual std::shared_ptr<PlantObject> clonePlant(const SDL_FPoint& root_point) = 0;

    virtual void setRootPoint(const SDL_FPoint& root_point) = 0;
    // virtual int render(uint64_t now_ms);
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

    virtual int damage(int damege_num);

    // 更新到下一帧的状态
    virtual int updatePlant(std::shared_ptr<Timer> timer) = 0;

    virtual int changeToStatic();
    // 获取动画的播放范围
    // render_point 为渲染的位置(想要绘制到一个从 (0, 0) 开始的纹理)
    // 最小使用多大的纹理可以完全渲染
    virtual int getAnimRange(float& width, float& height);
    // 仅在渲染到纹理时使用
    // 仅可对不需要渲染的模板植物使用
    // 因内部会修改动画渲染过程
    virtual int renderToTexture() = 0;

    // 提示放置植物的位置
    // 半透明绘制 IDLE 状态动画第一帧
    virtual int renderAlpha() = 0;

    int showAABB();

    int showShadow();

    virtual ~PlantObject() {};
};

#endif