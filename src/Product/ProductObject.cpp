#include "Product/ProductObject.h"
#include "Core/CoreVars.h"

ProductObject::ProductObject(const SDL_FRect& aabb) :
    GameObject(aabb),
    m_updater(nullptr),
    m_state(ProductState::Product_MOVE)
{}

int ProductObject::update()
{
    return m_updater->step(this, CoreVars::getInstance().timer.getDeltaTime());
}

ProductObject::~ProductObject()
{}