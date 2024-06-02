#ifndef ANIMRES_H
#define ANIMRES_H

#include <memory>
#include <string>
#include <vector>
#include <map>

#include "SDL.h"
#include "SDL_image.h"
#include "Resource/AnimLoader.h"

class AnimRes {
protected:
    // 动画文件路径 -> 动画数据
    std::map<std::string, std::shared_ptr<AnimLoader>> m_anims;
public:
    AnimRes() = default;

    // 根据动画文件的路径(限定在 reanim 格式)获取动画数据
    std::shared_ptr<AnimLoader> getAnimFrom(const std::string& file_path);

    // 释放指定的动画
    int freeAnim(const std::string& file_path);

    // 释放动画资源 必需在 SDL_Quit 之前调用
    void clean();

    ~AnimRes();
};

#endif