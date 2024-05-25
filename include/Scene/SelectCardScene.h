#ifndef SELECTCARDSCENE_H
#define SELECTCARDSCENE_H

#include "SceneObject.h"
#include "Plants/PlantObject.h"

class MapManager;
class CardManager;

class SelectCardScene : public SceneObject {
protected:

    // 游戏对象 manager
    std::shared_ptr<MapManager> m_mapManager;
    std::shared_ptr<CardManager> m_cardManager;
public:
    SelectCardScene(
        std::shared_ptr<MapManager> mapManager,
        std::shared_ptr<CardManager> cardManager
    );

    virtual SceneType getType() override;

    virtual int enterScene() override;

    virtual SceneType handleEvent(SDL_Event& event) override;

    virtual SceneType updateScene() override;

    virtual int exitScene() override;

    virtual int renderScene() override;
    
    ~SelectCardScene();
};




#endif