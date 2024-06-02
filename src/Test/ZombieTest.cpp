#include <thread>
#include "Core/CoreVars.h"
#include "Resource/ResVars.h"
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
        CoreVars::getInstance().timer.updateTime();

        /*************************************************************
         *                            lock                           *
        **************************************************************/
        handle_update_render_spinlock.lock();
        Managers::getInstance().zombieManager->updateZombie();

        // 清空屏幕
        SDL_SetRenderDrawColor(CoreVars::getInstance().renderer, 0, 10, 100, 255);
        SDL_RenderClear(CoreVars::getInstance().renderer);

        Managers::getInstance().zombieManager->renderZombie();

        /*************************************************************
         *                          unlock                           *
        **************************************************************/
        handle_update_render_spinlock.unlock();


        // 刷新屏幕
        SDL_RenderPresent(CoreVars::getInstance().renderer);

        // 帧率控制
        if (CoreVars::getInstance().timer.getDeltaTime() < FLUSH_DELAY)
        {
            SDL_Delay(FLUSH_DELAY - CoreVars::getInstance().timer.getDeltaTime());
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

    // 全局变量的初始化
    CoreVars::getInstance();
    ResVars::getInstance();
    Managers::getInstance();

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

            Managers::getInstance().sceneManager->handleEvent(event);

            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    QuitFlag = true;
                    break;
                case SDLK_SPACE:
                    if (CoreVars::getInstance().timer.isPause()) CoreVars::getInstance().timer.start();
                    else CoreVars::getInstance().timer.pause();
                    break;
                case SDLK_UP:
                    CoreVars::getInstance().camera.move(0, -5.0f);
                    break;
                case SDLK_DOWN:
                    CoreVars::getInstance().camera.move(0, 5.0f);
                    break;
                case SDLK_LEFT:
                    CoreVars::getInstance().camera.move(-5.0f, 0);
                    break;
                case SDLK_RIGHT:
                    CoreVars::getInstance().camera.move(5.0f, 0);
                    break;
                case SDLK_1:
                    Managers::getInstance().zombieManager->addZombie(ZombieType::ZombieNormal, 0, 7);
                    break;
                case SDLK_2:
                    Managers::getInstance().zombieManager->addZombie(ZombieType::ZombieNormal, 1, 7);
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
    ResVars::release();
    SDL_DestroyRenderer(CoreVars::getInstance().renderer);
    SDL_DestroyWindow(CoreVars::getInstance().window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}