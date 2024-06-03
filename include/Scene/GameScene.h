#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "SceneObject.h"
#include "Plants/PlantObject.h"

class GameScene : public SceneObject {
protected:
    // 游戏进程
    int m_cardInHandIdx;                    // 正在手中的卡槽编号
    PlantType m_plantInHandType;            // 正在手中的植物类型
    int m_mousePositionX, m_mousePositionY; // 鼠标移动的位置

    // 阳光产生间隔
    uint64_t m_dropSunIntervalTime;         // 随机阳光掉落的时间间隔
    uint64_t m_dropSunCountDown;            // 产生随机阳光的倒计时

public:
    GameScene();

    virtual SceneType getType() override;

    virtual int enterScene() override;

    virtual SceneType handleEvent(SDL_Event& event) override;

    virtual SceneType updateScene() override;

    virtual int exitScene() override;

    virtual int renderScene() override;

    virtual ~GameScene();

protected:
    void _settle_plant();
    void _put_back_card();

    void _on_LB_haveCardInHand(SDL_Event& event);
    void _on_LB_noCardInHand(SDL_Event& event);
};




#endif