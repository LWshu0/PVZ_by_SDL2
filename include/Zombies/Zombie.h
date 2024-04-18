#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "ZombieObject.h"

class Zombie :public ZombieObject {
private:
    /* data */
public:
    Zombie(
        std::shared_ptr<AnimLoader> loader,
        std::shared_ptr<Camera> camera,
        const SDL_FPoint& root_point
    );
    virtual std::shared_ptr<ZombieObject> cloneZombie(const SDL_FPoint& root_point) override;
    
    virtual int render() override;
    virtual int changeAnimState(AnimState to_state) override;

    virtual int changeZombieState(ZombieState to_state, std::shared_ptr<Timer> timer) override;
    virtual int attack(std::shared_ptr<Timer> timer) override;
    virtual int updateZombie(std::shared_ptr<Timer> timer) override;

    ~Zombie();
};

#endif