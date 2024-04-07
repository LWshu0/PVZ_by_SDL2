#ifndef ANIM_PEASHOOTERSINGLE_H
#define ANIM_PEASHOOTERSINGLE_H

#include "PlantObject.h"

class PeaShooterSingle : public PlantObject {
protected:
    // 眨眼控制
    bool is_blinking;
    uint64_t last_blink_ms;
    uint64_t delta_blink_ms;
public:
    PeaShooterSingle(
        std::shared_ptr<AnimLoader> loader,
        std::shared_ptr<Camera> camera,
        const SDL_FPoint& root_point
    );
    // 0 -> idel, 1 -> emit
    virtual int Play(uint64_t now_ms) override;
    virtual int changeAnimState(AnimState to_state) override;

    virtual int attack() override;

    ~PeaShooterSingle();
};

#endif