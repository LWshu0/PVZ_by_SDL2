#ifndef MAINSCENE_H
#define MAINSCENE_H

#include "Scene/SceneObject.h"

class MainScene:public SceneObject, public AnimPlayer
{
protected:

public:
    MainScene(
        std::shared_ptr<Timer> timer,
        std::shared_ptr<Camera> camera,
        std::shared_ptr<TextureRes> res,
        std::shared_ptr<AnimLoader> loader
    );

    virtual int changeAnimState(AnimState to_state) override;

    virtual int enterScene() override;

    virtual SceneType handleEvent(SDL_Event& event) override;

    virtual int updateScene() override;

    virtual int exitScene() override;

    virtual int renderScene() override;

    ~MainScene();
};

#endif