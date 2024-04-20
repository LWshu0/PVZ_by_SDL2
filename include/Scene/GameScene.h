#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "SceneObject.h"

class GameScene : public SceneObject {
protected:
    
public:
    GameScene(
        SDL_Renderer* renderer,
        std::shared_ptr<Timer> timer,
        std::shared_ptr<Camera> camera,
        std::shared_ptr<TextureRes> res
    );

    virtual SceneType getType() override;

    virtual int enterScene() override;

    virtual SceneType handleEvent(SDL_Event& event) override;

    virtual int updateScene() override;

    virtual int exitScene() override;

    virtual int renderScene() override;

    ~GameScene();
};




#endif