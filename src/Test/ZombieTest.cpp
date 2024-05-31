#include <thread>
#include "Core/GlobalVars.h"
#include "Scene/GameScene.h"
#include "Manager/MapManager.h"
#include "Manager/ProductManager.h"
#include "Manager/PlantManager.h"
#include "Manager/ZombieManager.h"
#include "Manager/TaskManager.h"
#include "Manager/CardManager.h"
#include "Manager/SceneManager.h"
#include "Manager/UsrInfoManager.h"

#define FLUSH_DELAY 1000 / 45

bool QuitFlag = false;

SpinLock handle_update_render_spinlock;

void RenderThread()
{

    while (!QuitFlag)
    {
        // 更新时钟
        GlobalVars::getInstance().timer.updateTime();

        /*************************************************************
         *                            lock                           *
        **************************************************************/
        handle_update_render_spinlock.lock();
        GlobalVars::getInstance().zombieManager->updateZombie();

        // 清空屏幕
        SDL_SetRenderDrawColor(GlobalVars::getInstance().renderer, 0, 10, 100, 255);
        SDL_RenderClear(GlobalVars::getInstance().renderer);

        GlobalVars::getInstance().zombieManager->renderZombie();

        /*************************************************************
         *                          unlock                           *
        **************************************************************/
        handle_update_render_spinlock.unlock();


        // 刷新屏幕
        SDL_RenderPresent(GlobalVars::getInstance().renderer);

        // 帧率控制
        if (GlobalVars::getInstance().timer.getDeltaTime() < FLUSH_DELAY)
        {
            SDL_Delay(FLUSH_DELAY - GlobalVars::getInstance().timer.getDeltaTime());
        }
    }

    SDL_Log("render thread quit!\n");
}

int main(int argc, char* args[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    if (!SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1")) { SDL_Log("error: fail to set SDL_HINT_IME_SHOW_UI"); }
    if (!SDL_SetHint(SDL_HINT_IME_SUPPORT_EXTENDED_TEXT, "1")) { SDL_Log("error: fail to set SDL_HINT_IME_SUPPORT_EXTENDED_TEXT"); }
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) { SDL_Log("error: fail to set SDL_HINT_RENDER_SCALE_QUALITY"); }

    // 创建管理者
    // UserInfoManager 必须第一个创建并完成数据读取
    // 后续的 Manager 可能会依赖 UserInfoManager 的数据
    GlobalVars::getInstance().usrinfoManager = std::make_shared<UsrInfoManager>();
    GlobalVars::getInstance().usrinfoManager->read();

    GlobalVars::getInstance().mapManager = std::make_shared<MapManager>();
    GlobalVars::getInstance().productManager = std::make_shared<ProductManager>(100, 100);
    GlobalVars::getInstance().plantManager = std::make_shared<PlantManager>();
    GlobalVars::getInstance().zombieManager = std::make_shared<ZombieManager>();
    GlobalVars::getInstance().taskManager = std::make_shared<TaskManager>();
    GlobalVars::getInstance().cardManager = std::make_shared<CardManager>();
    GlobalVars::getInstance().cardManager->initilizeManagers();
    GlobalVars::getInstance().sceneManager = std::make_shared<SceneManager>();

    std::thread render_thread(RenderThread);

    SDL_Event event;
    while (!QuitFlag)
    {
        if (SDL_WaitEvent(&event))
        {
            /*************************************************************
            *                            lock                           *
            **************************************************************/
            handle_update_render_spinlock.lock();

            if (event.type == SDL_QUIT)
            {
                QuitFlag = true;
            }

            GlobalVars::getInstance().sceneManager->handleEvent(event);

            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    QuitFlag = true;
                    break;
                case SDLK_SPACE:
                    if (GlobalVars::getInstance().timer.isPause()) GlobalVars::getInstance().timer.start();
                    else GlobalVars::getInstance().timer.pause();
                    break;
                case SDLK_UP:
                    GlobalVars::getInstance().camera.move(0, -5.0f);
                    break;
                case SDLK_DOWN:
                    GlobalVars::getInstance().camera.move(0, 5.0f);
                    break;
                case SDLK_LEFT:
                    GlobalVars::getInstance().camera.move(-5.0f, 0);
                    break;
                case SDLK_RIGHT:
                    GlobalVars::getInstance().camera.move(5.0f, 0);
                    break;
                case SDLK_1:
                    GlobalVars::getInstance().zombieManager->addZombie(ZombieType::ZombieNormal, 0, 7);
                    break;
                case SDLK_2:
                    GlobalVars::getInstance().zombieManager->addZombie(ZombieType::ZombieNormal, 1, 7);
                    break;
                default:
                    break;
                }
            }
            /*************************************************************
            *                          unlock                           *
            **************************************************************/
            handle_update_render_spinlock.unlock();
        }
    }

    render_thread.join();

    SDL_DestroyRenderer(GlobalVars::getInstance().renderer);
    SDL_DestroyWindow(GlobalVars::getInstance().window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}