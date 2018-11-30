#include "ColorComponent.h"
#include "BaseComponent.h"
#include "SDL.h"

ColorComponent::ColorComponent() : BaseComponent()
{
    Red = 0;
    Green = 0;
    Blue = 0;
    Alpha = SDL_ALPHA_OPAQUE;
}

ColorComponent::~ColorComponent()
{
    //dtor
}
