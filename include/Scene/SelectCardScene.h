#ifndef SELECTCARDSCENE_H
#define SELECTCARDSCENE_H

#include "SceneObject.h"
#include "Plants/PlantObject.h"

class MapManager;
class CardManager;

class SelectCardScene : public SceneObject {
protected:
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