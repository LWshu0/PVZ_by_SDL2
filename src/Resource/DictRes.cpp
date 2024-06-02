#include "Resource/DictRes.h"

std::shared_ptr<Dictionary> DictRes::getDictFrom(
    const std::string& imageFilePath,
    const std::string& wordFilePath,
    const SDL_Color& color
)
{
    std::string name = imageFilePath
        + std::to_string(color.r)
        + std::to_string(color.g)
        + std::to_string(color.b)
        + std::to_string(color.a);
    
    auto iter = m_anims.find(name);
    if (iter != m_anims.end())
    {
        return iter->second;
    }
    std::shared_ptr<Dictionary> loader = std::make_shared<Dictionary>(imageFilePath, wordFilePath, color);
    m_anims[name] = loader;
    return loader;
}

int DictRes::freeDict(
    const std::string& imageFilePath,
    const SDL_Color& color
)
{
    std::string name = imageFilePath
        + std::to_string(color.r)
        + std::to_string(color.g)
        + std::to_string(color.b)
        + std::to_string(color.a);
    
    auto iter = m_anims.find(name);
    if (iter != m_anims.end())
    {
        m_anims.erase(iter);
        return 0;
    }
    return -1;
}

void DictRes::clean()
{
    m_anims.clear();
}

DictRes::~DictRes()
{
    clean();
}