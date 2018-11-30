#ifndef SDLRECTANGLECOMPONENT_H
#define SDLRECTANGLECOMPONENT_H

#include "BaseComponent.h"
#include "ColorComponent.h"

class SdlRectangleComponent : public BaseComponent
{
    public:
        SdlRectangleComponent();
        virtual ~SdlRectangleComponent();
        int Width;
        int Height;
        int Filled;
        ColorComponent Color;
};

#endif // SDLRECTANGLECOMPONENT_H
