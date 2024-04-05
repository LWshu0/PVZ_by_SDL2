#ifndef ANIM_PEASHOOTERSINGLE_H
#define ANIM_PEASHOOTERSINGLE_H

#include "Core/AnimPlayer.h"

class Anim_PeaShooterSingle : public AnimPlayer {
protected:
    // 眨眼控制
    bool is_blinking;
    uint64_t last_blink_ms;
    uint64_t delta_blink_ms;
public:
    Anim_PeaShooterSingle(std::shared_ptr<AnimLoader> loader, std::shared_ptr<Camera> camera, const SDL_FPoint& init_point);
    // 0 -> idel, 1 -> emit
    virtual int Play(uint64_t now_ms) override;
    virtual int ChangeAnim(AnimState to_state) override;

    ~Anim_PeaShooterSingle();
};

#endif