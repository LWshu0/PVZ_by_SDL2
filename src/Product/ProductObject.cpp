#include "Product/ProductObject.h"
ProductObject::ProductObject(const SDL_FRect& aabb) :
    GameObject(aabb),
    m_updater(nullptr),
    m_state(ProductState::Product_MOVE)
{}

ProductObject::~ProductObject()
{}