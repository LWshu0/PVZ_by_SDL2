#ifndef OBJ_PLANT_H
#define OBJ_PLANT_H

#include "Core/GameObject.h"
#include "Core/AnimPlayer.h"

class PlantObject: public GameObject, public AnimPlayer {
public:
    // 
    int m_HP;
    int m_lastAttackMilliSecond;
public:
    PlantObject(
        std::shared_ptr<AnimLoader> loader,
        std::shared_ptr<Camera> camera,
        const SDL_FPoint& init_point,
        int HP,
        const SDL_FRect& aabb
    );

    PlantObject(
        std::shared_ptr<AnimLoader> loader,
        std::shared_ptr<Camera> camera,
        const SDL_FPoint& init_point,
        int HP,
        const SDL_FPoint& root_point,
        float width,
        float height
    );
    // virtual int Play(uint64_t now_ms);
    // virtual int changeDamageState(DamageState to_state);
    // virtual int changeAnimState(AnimState to_state);
    // virtual SDL_FPoint GetPosition();
    // virtual void setPlayPosition(const SDL_FPoint& point);

    // 
    virtual int attack() = 0;

    virtual int damage(int damege);
    //
    // virtual int update(uint64_t now_ms) override;

    virtual ~PlantObject() {};
};

#endif