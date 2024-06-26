#ifndef PLANTOBJECT_H
#define PLANTOBJECT_H

#include "Core/Timer.h"
#include "Core/GameObject.h"
#include "Core/AnimPlayer.h"
#include "Product/ProductObject.h"
#include "Core/ExtSDL.h"

#include <memory>

enum PlantType {
    PlantPeaShooter1,
    SunFlower_,
    MaxPlantType
};

enum PlantState {
    Plant_IDLE,         // IDLE 状态播放 IDLE 动画
    Plant_ATTACK,       // 在攻击逻辑时播放攻击动画
    Plant_DEAD,         // 植物死亡状态, 播放死亡动画
    Plant_DELETE        // 植物删除状态, 其对象可以被销毁
};

class PlantObject : public GameObject {
protected:
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

    // 动画
    AnimPlayer m_animPlayer;
    // 植物影子
    SDL_Texture* m_shadow;
    SDL_FRect m_shadowRange;
public:
    PlantObject(
        std::shared_ptr<AnimLoader> loader,
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
        std::shared_ptr<AnimLoader> loader, // 动画资源载入器(内部有完整的动画数据)
        const SDL_FPoint& init_point,       // 动画播放的起始点(左上角)
        const SDL_FPoint& offset_abbb,      // aabb 偏移(相对于 init_point)
        float aabb_width,                   // 碰撞箱宽度
        float aabb_height,                  // 碰撞箱高度
        const SDL_FPoint& offset_shadow,    // 阴影偏移(相对于 init_point)
        float shadow_width,                 // 阴影宽度
        float shadow_height,                // 阴影高度
        int HP,                             // 植物生命值
        PlantState state,                   // 植物初始状态
        int reload_ms,                      // 攻击间隔
        uint64_t windup_duration            // 攻击前摇时间(毫秒), 从攻击动画开始播放到实际攻击逻辑执行的时间
    );

    /**
     *@brief 根据已有的对象构造一个新的对象, 新对象仅位置与原对象不同
     * 
     * @param root_point 植物的根部坐标, 该位置用于对齐到地图上安置植物的点
     * @return std::shared_ptr<PlantObject> 构造的新对象
     */
    virtual std::shared_ptr<PlantObject> clone(const SDL_FPoint& root_point) = 0;

    /**
     *@brief 将物体的动画播放位置设置为 real_point, 同时一并修改相关的各种坐标, 比如 AABB 碰撞箱的坐标等. 
     *       该函数将在 setRootPoint 函数中调用
     * @param real_point 动画播放位置
     */
    void setObjectPosition(const SDL_FPoint& real_point);
    
    /**
     *@brief 根据植物根部坐标设置植物的位置, 因为不同的植物的根部位置不同, 所以需要每个植物各自计算合适的偏移.
     *       使得可以得到正确的 real_point 即动画播放位置, 然后调用 setObjectPosition 函数设置位置.
     * @param root_point 植物预期的根部坐标
     */
    virtual void setRootPoint(const SDL_FPoint& root_point) = 0;

    /**
     *@brief 改变当前植物状态, 同时适当的变换动画状态. 如果当前植物状态与要改变的植物状态相同,
     *       则直接返回, 不做任何更改
     * @param to_state 要改变的植物状态
     * @return int 改变成功返回 0, 改变失败(即当前状态与期望状态一致)返回 -1
     */
    virtual int setPlantState(PlantState to_state) = 0;

    // 判断植物是否死亡, 即 HP 小于等于 0
    inline bool isDead() { return m_HP <= 0; }

    /**
     *@brief 判断敌人是否处于自己的攻击范围内
     * 
     * @param enemy_aabb 敌人的 AABB 包围盒
     * @return true 如果在攻击范围内
     * @return false 如果不在攻击范围内
     */
    virtual bool inAttackRange(const SDL_FRect& enemy_aabb) = 0;

    // 对植物造成伤害, 使植物 HP 减少
    virtual void damage(int damege_num);

    /*  将植物的状态改为静止状态(该状态并不在 PlantState 中), 静止状态专门用于显示静态的植物图像.
        同时, 该函数还要设置动画渲染的位置, 使 renderStatic 函数可以把植物图像渲染在纹理中间位置
        每个植物都可以自定义一个合适的动画状态与渲染位置, 使得贴图更为美观, 实用.
        例如: 植物卡片上的静态贴图, 鼠标拿起卡片时跟随鼠标移动的贴图, 将要放置的植物在地图上的虚影
    */
    virtual int changeToStatic() = 0;

    /*  获取静态状态下动画的播放范围的宽度和高度, 这个范围一般应该大于有效的动画范围
        使用这个函数获取的大小去创建一个纹理, 该纹理必需可以渲染动画帧中所有的元素
        每个植物应该重写该函数使得获取的大小刚好稍大于能容纳所有元素的最小范围
        p.s. 卡片管理者使用该函数确定需要创建的纹理大小
    */
    virtual int getAnimRange(float& width, float& height) = 0;

    /*  渲染静态状态下的植物图像(一般为 IDLE 状态的第一帧)
        1. 该函数将植物图像渲染到一个纹理上, 用于在游戏中拿起卡片时, 植物图像跟随鼠标移动
        2. 该函数将在创建植物卡片贴图时, 把1中创建的植物纹理渲染到卡片背景纹理上, 创建植物的卡片纹理
        3. 在游戏中, 该函数将渲染一个半透明的虚影到植物将要放置的位置
    */
    virtual int renderStatic(uint8_t alpha = 255) = 0;

    // 显示植物的碰撞箱
    int showAABB();
    // 显示植物阴影
    int showShadow();

    virtual ~PlantObject() {};
};

#endif