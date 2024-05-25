#ifndef SELECT2GAME_H
#define SELECT2GAME_H

#include "SceneObject.h"

class MapManager;

class Select2Game :public SceneObject {
protected:
    // 游戏对象 manager
    std::shared_ptr<MapManager> m_mapManager;
public:
    Select2Game(
        std::shared_ptr<MapManager> mapManager
    );

    virtual SceneType getType() override;

    virtual int enterScene() override;

    virtual SceneType handleEvent(SDL_Event& event) override;

    virtual SceneType updateScene() override;

    virtual int exitScene() override;

    virtual int renderScene() override;

    ~Select2Game();
};

#endif