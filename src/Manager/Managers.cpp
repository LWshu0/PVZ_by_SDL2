#include "Manager/Managers.h"
Managers& Managers::getInstance()
{
    static Managers instance;
    return instance;
}
