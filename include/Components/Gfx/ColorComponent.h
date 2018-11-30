#ifndef COLORCOMPONENT_H
#define COLORCOMPONENT_H

#include "BaseComponent.h"
#include "SDL.h"

class ColorComponent : public BaseComponent
{
    public:
        ColorComponent();
        virtual ~ColorComponent();

        Uint8 Red;
        Uint8 Green;
        Uint8 Blue;
        Uint8 Alpha;
};

#endif // COLORCOMPONENT_H
