#include "Core/Particle/Particle_2d.h"
#include "Core/Particle/ParticleSetters_2d.h"
#include "Core/Particle/ParticleUpdaters_2d.h"
#include "Core/Particle/ParticleRenderers_2d.h"
#include "Core/GlobalVars.h"

#define FLUSH_DELAY 1000 / 45

int main(int argc, char* args[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);

    // builder
    std::shared_ptr<ParticleSetter_2d> setter_center = std::make_shared<ParticleSetter_2d_default>(
        200
    );
    std::shared_ptr<ParticleSetter_2d> setter_other = std::make_shared<ParticleSetter_2d_explosion>(
        300,
        100
    );
    // updater
    std::shared_ptr<ParticleUpdater_2d> updater0 = std::make_shared<ParticleUpdater_2d_Lifetime>();
    std::shared_ptr<ParticleUpdater_2d> updater1 = std::make_shared<ParticleUpdater_2d_Speed>();
    std::shared_ptr<ParticleUpdater_2d> updater3 = std::make_shared<ParticleUpdater_2d_Rotate>(40.0f);
    std::shared_ptr<ParticleUpdater_2d> updater4 = std::make_shared<ParticleUpdater_2d_Size>(0.5f, 0.5f);
    std::shared_ptr<ParticleUpdater_2d> updater8 = std::make_shared<ParticleUpdater_2d_Force>(0.0f, 150.0f);
    // renderer
    std::shared_ptr<ParticleRenderer_2d> render0 = std::make_shared<ParticleRenderer_2d_Sprite>(
        GlobalVars::getInstance().textureRes.getTextureFrom("particles/Pea_particles.png"), 3);

    std::shared_ptr<ParticleRenderer_2d> render1 = std::make_shared<ParticleRenderer_2d_Sprite>(
        GlobalVars::getInstance().textureRes.getTextureFrom("particles/pea_splats.png"), 4);

    // emitter
    ParticleEmitter_2d emitter(
        100.0f, 100.0f,         // 发射器位置
        1,                    // 最大粒子数量
        3000,            // 发射器生命值
        3000,                   // 发射间隔
        setter_center,               // 粒子构造器
        { updater0 },   // 粒子更新器
        render1                 // 粒子渲染器
    );
    emitter.setOneShoot(true);

    ParticleEmitter_2d emitter_particle(
        100.0f, 100.0f,
        5,
        3000,
        3000,
        setter_other,
        { updater0, updater1, updater4, updater8 },
        render0
    );
    emitter_particle.setOneShoot(true);
    emitter_particle.initilize(100.0f, 100.0f);

    bool quit = false;
    SDL_Event event;
    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
            else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
            {
                quit = true;
            }

        }
        // 更新时钟
        GlobalVars::getInstance().timer.updateTime();
        // 更新粒子发射器
        emitter.update();
        emitter_particle.update();
        if (!emitter.valid() && !emitter_particle.valid())
        {
            emitter.initilize(100.0f, 100.0f);
            emitter_particle.initilize(100.0f, 100.0f);
        }

        // 清空屏幕
        SDL_SetRenderDrawColor(GlobalVars::getInstance().renderer, 0, 10, 100, 255);
        SDL_RenderClear(GlobalVars::getInstance().renderer);

        // 渲染图形
        SDL_SetRenderDrawColor(GlobalVars::getInstance().renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(GlobalVars::getInstance().renderer, 0, 100, 200, 100);
        SDL_RenderDrawLine(GlobalVars::getInstance().renderer, 100, 0, 100, 200);
        // 渲染粒子发射器
        emitter.render();
        emitter_particle.render();

        // 刷新屏幕
        SDL_RenderPresent(GlobalVars::getInstance().renderer);

        // 帧率控制
        if (GlobalVars::getInstance().timer.getDeltaTime() < FLUSH_DELAY)
        {
            SDL_Delay(FLUSH_DELAY - GlobalVars::getInstance().timer.getDeltaTime());
        }
    }

    SDL_DestroyRenderer(GlobalVars::getInstance().renderer);
    SDL_DestroyWindow(GlobalVars::getInstance().window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}