#include "Scene/GameScene.h"
#include <thread>
#include "SDL_image.h"
#include "Core/Timer.h"
#include "Core/Camera.h"
#include "Core/TextureRes.h"
#include "Manager/MapManager.h"
#include "Manager/BulletManager.h"
#include "Manager/PlantManager.h"
#include "Manager/ZombieManager.h"
#include "Manager/TaskManager.h"
#include "Manager/CardManager.h"
#include "Manager/CollectionManager.h"
#include "Manager/SceneManager.h"

#define FLUSH_DELAY 1000 / 45

// 全局变量 适配分辨率
SDL_Window* pvz_window = nullptr;
SDL_Renderer* pvz_renderer = nullptr;
unsigned int pvz_window_width = 800;
unsigned int pvz_window_height = 600;
bool QuitFlag = false;
// float pvz_window_scale = 1.0f;
std::shared_ptr<Timer> pvz_timer;
std::shared_ptr<Camera> pvz_camera;
// int pvz_card_width = 53;
// int pvz_card_height = 71;
std::shared_ptr<TextureRes> texture_res;
std::shared_ptr<MapManager> map_manager;
std::shared_ptr<BulletManager> bullet_manager;
std::shared_ptr<PlantManager> plant_manager;
std::shared_ptr<ZombieManager> zombie_manager;
std::shared_ptr<TaskManager> task_manager;
std::shared_ptr<CardManager> card_manager;
std::shared_ptr<CollectionManager> collection_manager;
std::shared_ptr<SceneManager> scene_manager;

SpinLock handle_update_render_spinlock;

void RenderThread()
{

    while (!QuitFlag)
    {
        // 更新时钟
        pvz_timer->updateTime();

        /*************************************************************
         *                            lock                           *
        **************************************************************/
        handle_update_render_spinlock.lock();
        // 更新场景
        scene_manager->updateScene();

        // 清空屏幕
        SDL_SetRenderDrawColor(pvz_renderer, 0, 10, 100, 255);
        SDL_RenderClear(pvz_renderer);

        // 渲染场景
        scene_manager->renderScene();
        /*************************************************************
         *                          unlock                           *
        **************************************************************/
        handle_update_render_spinlock.unlock();

        // 刷新屏幕
        SDL_RenderPresent(pvz_renderer);

        // 帧率控制
        if (pvz_timer->getDeltaTime() < FLUSH_DELAY)
        {
            SDL_Delay(FLUSH_DELAY - pvz_timer->getDeltaTime());
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

    pvz_window = SDL_CreateWindow("PVZ",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        pvz_window_width, pvz_window_height,
        SDL_WINDOW_SHOWN);
    if (pvz_window == nullptr) { SDL_Log("fail to create a window"); return 2; }

    pvz_renderer = SDL_CreateRenderer(pvz_window, -1, SDL_RENDERER_ACCELERATED);
    if (pvz_renderer == nullptr) { SDL_Log("fail to create a renderer"); return 2; }

    SDL_SetRenderDrawBlendMode(pvz_renderer, SDL_BLENDMODE_BLEND);

    // 创建基础对象
    texture_res = std::make_shared<TextureRes>(pvz_renderer, "resource/resource.xml", "reanim");
    pvz_timer = std::make_shared<Timer>();
    pvz_camera = std::make_shared<Camera>(0, 0, 800, 600);

    // 创建管理者
    map_manager = std::make_shared<MapManager>(pvz_renderer, texture_res, pvz_camera);
    bullet_manager = std::make_shared<BulletManager>(pvz_renderer, texture_res, pvz_camera, pvz_timer, 100);
    plant_manager = std::make_shared<PlantManager>(pvz_renderer, texture_res, pvz_camera, pvz_timer);
    zombie_manager = std::make_shared<ZombieManager>(pvz_renderer, texture_res, pvz_camera, pvz_timer);
    task_manager = std::make_shared<TaskManager>(pvz_timer);
    card_manager = std::make_shared<CardManager>(pvz_renderer, pvz_timer, pvz_camera, texture_res);
    collection_manager = std::make_shared<CollectionManager>(pvz_renderer, texture_res, pvz_camera, pvz_timer, 50);
    scene_manager = std::make_shared<SceneManager>(
        pvz_renderer, texture_res, pvz_camera, pvz_timer,
        map_manager, bullet_manager, plant_manager, zombie_manager, task_manager, card_manager, collection_manager
    );

    // 关联管理者
    plant_manager->initilizeManagers(map_manager, bullet_manager, zombie_manager);
    zombie_manager->initilizeManagers(map_manager, bullet_manager, plant_manager);
    task_manager->initilizeManagers(map_manager, zombie_manager);
    card_manager->initilizeManagers(map_manager, plant_manager, collection_manager);
    collection_manager->initilizeManagers(map_manager);

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

            scene_manager->handleEvent(event);

            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    QuitFlag = true;
                    break;
                case SDLK_SPACE:
                    if (pvz_timer->isPause()) pvz_timer->start();
                    else pvz_timer->pause();
                    break;
                case SDLK_UP:
                    pvz_camera->move(0, -5.0f);
                    break;
                case SDLK_DOWN:
                    pvz_camera->move(0, 5.0f);
                    break;
                case SDLK_LEFT:
                    pvz_camera->move(-5.0f, 0);
                    break;
                case SDLK_RIGHT:
                    pvz_camera->move(5.0f, 0);
                    break;
                case SDLK_1:
                    zombie_manager->addZombie(ZombieType::ZombieNormal, 0, 7);
                    break;
                case SDLK_2:
                    zombie_manager->addZombie(ZombieType::ZombieNormal, 1, 7);
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

    // 取消关联(因存在循环引用, 需要释放内部的智能指针)
    plant_manager->releaseManagers();
    zombie_manager->releaseManagers();
    task_manager->releaseManagers();
    card_manager->releaseManagers();
    collection_manager->releaseManagers();
    
    SDL_DestroyRenderer(pvz_renderer);
    SDL_DestroyWindow(pvz_window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}