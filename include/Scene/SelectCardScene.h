#ifndef SELECTCARDSCENE_H
#define SELECTCARDSCENE_H

#include "SceneObject.h"
#include "Plants/PlantObject.h"
#include "Core/UI/wsRectangle.h"
#include "Core/Dictionary.h"

class MapManager;
class CardManager;

class SelectCardScene : public SceneObject {
protected:
    bool m_enableStartButton;
    bool m_hoverStartButton;
    wsRectangle m_startButtonRange;
    SDL_Texture* m_startButtonBK;
    SDL_Texture* m_startButtonDisable;
    SDL_Texture* m_startButtonBloom;
    Dictionary m_enableFont;
    Dictionary m_disableFont;
public:
    SelectCardScene();

    virtual SceneType getType() override;

    virtual int enterScene() override;

    virtual SceneType handleEvent(SDL_Event& event) override;

    virtual SceneType updateScene() override;

    virtual int exitScene() override;

    virtual int renderScene() override;
    
    virtual ~SelectCardScene();
};




#endif