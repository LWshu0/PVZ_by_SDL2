#ifndef CARDMANAGER_H
#define CARDMANAGER_H

#include <vector>
#include <memory>

#include "SDL.h"
#include "Core/Timer.h"
#include "Core/Camera.h"
#include "Core/TextureRes.h"

#include "Plants/PlantObject.h"

class MapManager;
class PlantManager;

class CardManager {
protected:
    SDL_Renderer* m_renderer;
    std::shared_ptr<Timer> m_timer;
    std::shared_ptr<Camera> m_camera;
    std::shared_ptr<TextureRes> m_textureRes;
    // 植物纹理
    std::vector<SDL_Texture*> m_plantImageTexture;
    // 卡片纹理
    std::vector<SDL_Texture*> m_plantCardTexture;

    // 关联 manager
    std::shared_ptr<MapManager> m_mapManager;
    std::shared_ptr<PlantManager> m_plantManager;

public:
    CardManager(
        SDL_Renderer* renderer,
        std::shared_ptr<Timer> timer,
        std::shared_ptr<Camera> camera,
        std::shared_ptr<TextureRes> res
    );

    // 关联 manager 初始化 texture
    int initilizeManagers(
        std::shared_ptr<MapManager> mapManager,
        std::shared_ptr<PlantManager> plantManager
    );

    int renderCard();

    void releaseManagers();

    ~CardManager();
};



#endif