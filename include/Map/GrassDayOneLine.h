#ifndef GRASSDAYONELINE_H
#define GRASSDAYONELINE_H

#include "Map/MapObject.h"

class GrassDayOneLine :public MapObject {
protected:
    int m_grassWidth, m_grassHeight;
    SDL_Texture* m_oneLineGrassTexture;
public:
    GrassDayOneLine();

    virtual int update() override;
    virtual int render() override;
    
    virtual ~GrassDayOneLine();
};

#endif