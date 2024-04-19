#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H
#include <memory>

#include "SDL.h"
#include "Core/Timer.h"
#include "Core/Camera.h"
#include "Core/TextureRes.h"
#include "Core/AnimLoader.h"
#include "Core/AnimPlayer.h"

enum SceneType {
    Scene_MainScene,
    Scene_MaxSceneIdx
};

class SceneObject {
protected:
    std::shared_ptr<Timer> m_timer;
    std::shared_ptr<Camera> m_camera;
    std::shared_ptr<TextureRes> m_textureRes;
public:
    SceneObject(
        std::shared_ptr<Timer> timer,
        std::shared_ptr<Camera> camera,
        std::shared_ptr<TextureRes> res
        ) :
        m_timer(timer),
        m_camera(camera),
        m_textureRes(res)
    {};

    virtual int enterScene() = 0;

    virtual SceneType handleEvent(SDL_Event& event) = 0;

    virtual int updateScene() = 0;

    virtual int exitScene() = 0;

    virtual int renderScene() = 0;

    virtual ~SceneObject() {};
};

#endif