#ifndef RESVARS_H
#define RESVARS_H

#include "Resource/TextureRes.h"
#include "Resource/AnimRes.h"
#include "Resource/DictRes.h"

class ResVars {
public:
    TextureRes textureRes;
    AnimRes animRes;
    DictRes dictRes;

    static ResVars& getInstance();
    // 释放资源
    static void release();
    // 禁止复制和赋值
    ResVars(ResVars const&) = delete;
    void operator=(ResVars const&) = delete;
    
private:
    ResVars();
};

#endif