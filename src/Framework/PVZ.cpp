#include "Scene/GameScene.h"
#include <thread>
#include "SDL_image.h"
#include "Core/Timer.h"
#include "Core/Camera.h"
#include "Core/TextureRes.h"
#include "Manager/MapManager.h"
#include "Manager/BulletManager.h"
#include "Manager/PlantManager.h"

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
SDL_Texture* bk_img = nullptr;

void RenderThread()
{

    while (!QuitFlag)
    {
        pvz_timer->updateTime();
        // 更新物体状态
        plant_manager->updatePlants();
        bullet_manager->updateBullets(pvz_timer->getDeltaTime());
        // ...

        // 清空屏幕
        SDL_SetRenderDrawColor(pvz_renderer, 0, 10, 100, 255);
        SDL_RenderClear(pvz_renderer);

        // 渲染图形
        SDL_SetRenderDrawColor(pvz_renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(pvz_renderer, (int)(pvz_camera->getRenderX(0)), (int)(pvz_camera->getRenderY(100)), (int)(pvz_camera->getRenderX(200)), (int)(pvz_camera->getRenderY(100)));
        SDL_RenderDrawLine(pvz_renderer, (int)(pvz_camera->getRenderX(100)), (int)(pvz_camera->getRenderY(0)), (int)(pvz_camera->getRenderX(100)), (int)(pvz_camera->getRenderY(200)));

        map_manager->renderMap();
        plant_manager->renderPlants();
        bullet_manager->renderBullets();
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

    texture_res = std::make_shared<TextureRes>(pvz_renderer, "resource/resource.xml", "reanim");
    pvz_timer = std::make_shared<Timer>();
    pvz_camera = std::make_shared<Camera>(0, 0, 800, 600);

    map_manager = std::make_shared<MapManager>(pvz_renderer, texture_res, pvz_camera);
    map_manager->setMap(0.0f, 0.0f, MapType::MapGrassDayOneLine);

    bullet_manager = std::make_shared<BulletManager>(pvz_renderer, texture_res, pvz_camera, 100);

    plant_manager = std::make_shared<PlantManager>(pvz_renderer, texture_res, pvz_camera, pvz_timer, map_manager, bullet_manager);
    plant_manager->initilizePlants();

    if (0 == plant_manager->addPlant(PlantType::PlantPeaShooter1, 0, 0)) { SDL_Log("add at (0, 0)\n"); }
    if (0 == plant_manager->addPlant(PlantType::PlantPeaShooter1, 0, 1)) { SDL_Log("add at (0, 1)\n"); }
    if (0 == plant_manager->addPlant(PlantType::PlantPeaShooter1, 1, 1)) { SDL_Log("add at (1, 1)\n"); }
    if (0 == plant_manager->removePlant(1, 1)) { SDL_Log("remove at (1, 1)\n"); }

    std::thread render_thread(RenderThread);

    SDL_Event event;
    while (!QuitFlag)
    {
        if (SDL_WaitEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                QuitFlag = true;
            }
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
                    plant_manager->doDamage(0, 0, 50);
                    break;
                case SDLK_2:

                    break;
                default:
                    break;
                }
            }

        }

    }

    render_thread.join();

    SDL_DestroyRenderer(pvz_renderer);
    SDL_DestroyWindow(pvz_window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}