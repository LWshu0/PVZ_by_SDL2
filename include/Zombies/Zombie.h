#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "ZombieObject.h"

class Zombie :public ZombieObject {
private:
    /* data */
public:
    Zombie(
        std::shared_ptr<AnimLoader> loader,
        const SDL_FPoint& root_point
    );
    virtual std::shared_ptr<ZombieObject> clone(const SDL_FPoint& root_point) override;
    
    virtual int render() override;

    virtual int setZombieState(ZombieState to_state) override;
    virtual int attack() override;
    virtual int update() override;
    
    virtual ~Zombie();
};

#endif