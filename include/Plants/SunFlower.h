#ifndef SUNFLOWER_H
#define SUNFLOWER_H

#include "PlantObject.h"

class SunFlower : public PlantObject {
protected:
    // 眨眼控制
    bool is_blinking;
    uint64_t last_blink_ms;
    uint64_t delta_blink_ms;
public:
    SunFlower(
        std::shared_ptr<AnimLoader> loader,
        const SDL_FPoint& root_point
    );

    virtual std::shared_ptr<PlantObject> clonePlant(const SDL_FPoint& root_point) override;
    virtual void setRootPoint(const SDL_FPoint& root_point) override;
    virtual int setPlantState(PlantState to_state) override;

    virtual bool inAttackRange(const SDL_FRect& enemy_aabb) override;
    virtual BulletType attack() override;

    virtual int updatePlant() override;

    virtual int changeToStatic() override;
    virtual int getAnimRange(float& width, float& height) override;
    virtual int renderStatic(uint8_t alpha = 255) override;
    virtual int render() override;

    ~SunFlower();
};

#endif