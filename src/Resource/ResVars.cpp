#include "Resource/ResVars.h"

ResVars& ResVars::getInstance()
{
    static ResVars instance;
    return instance;
}

void ResVars::release()
{
    SDL_Log("resource release!\n");
    getInstance().textureRes.clean();
    getInstance().animRes.clean();
    getInstance().dictRes.clean();
}

ResVars::ResVars()
{
    textureRes.initilize("resource/resource.xml", "reanim");
}