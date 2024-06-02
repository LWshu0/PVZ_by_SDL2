#ifndef WINSCENE_H
#define WINSCENE_H

#include "SceneObject.h"
#include "Core/UI/wsRectangle.h"
#include "Resource/Dictionary.h"

class WinScene : public SceneObject
{
protected:
    SDL_Texture* m_bkTexture;
    SDL_Texture* m_buttonIdle;
    SDL_Texture* m_buttonHover;
    SDL_Texture* m_buttonNextTexture;
    SDL_Texture* m_buttonQuitTexture;

    wsRectangle m_buttonNextRange;
    wsRectangle m_buttonQuitRange;

    SDL_Rect m_cardRect;

    Dictionary m_buttonFont;
    Dictionary m_inforFont;
public:
    WinScene();

    virtual SceneType getType() override;

    virtual int enterScene() override;

    virtual SceneType handleEvent(SDL_Event& event) override;

    virtual SceneType updateScene() override;

    virtual int exitScene() override;

    virtual int renderScene() override;

    virtual ~WinScene();
};



#endif