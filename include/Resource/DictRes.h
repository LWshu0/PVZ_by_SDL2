#ifndef DICTRES_H
#define DICTRES_H

#include <memory>
#include <string>
#include <vector>
#include <map>

#include "SDL.h"
#include "SDL_image.h"
#include "Resource/Dictionary.h"

class DictRes {
protected:
    // 字体文件路径+颜色 -> 字体数据
    std::map<std::string, std::shared_ptr<Dictionary>> m_anims;
public:
    DictRes() = default;

    // 根据字体的路径, 颜色获取字体数据
    std::shared_ptr<Dictionary> getDictFrom(
        const std::string& imageFilePath,
        const std::string& wordFilePath,
        const SDL_Color& color = SDL_Color{ 255,255,255,255 }
    );

    // 释放指定的字体
    int freeDict(
        const std::string& imageFilePath,
        const SDL_Color& color = SDL_Color{ 255,255,255,255 }
    );

    // 释放资源 必需在 SDL_Quit 之前调用
    void clean();

    ~DictRes();
};

#endif