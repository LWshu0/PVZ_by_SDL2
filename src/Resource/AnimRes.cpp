#include "Resource/AnimRes.h"

std::shared_ptr<AnimLoader> AnimRes::getAnimFrom(const std::string& file_path)
{
    auto iter = m_anims.find(file_path);
    if (iter != m_anims.end())
    {
        return iter->second;
    }
    std::shared_ptr<AnimLoader> loader = std::make_shared<AnimLoader>(file_path);
    m_anims[file_path] = loader;
    return loader;
}

int AnimRes::freeAnim(const std::string& file_path)
{
    auto iter = m_anims.find(file_path);
    if (iter != m_anims.end())
    {
        m_anims.erase(iter);
        return 0;
    }
    return -1;
}

void AnimRes::clean()
{
    m_anims.clear();
}

AnimRes::~AnimRes()
{
    clean();
}