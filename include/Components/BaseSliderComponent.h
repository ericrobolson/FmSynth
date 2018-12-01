#ifndef BASESLIDERCOMPONENT_H
#define BASESLIDERCOMPONENT_H

#include "BaseComponent.h"

class BaseSliderComponent : public BaseComponent
{
    public:
        BaseSliderComponent();
        virtual ~BaseSliderComponent();

        static const int MaxValue = 100;
        static const int MinValue = 0;

        int xOffset;
        int yOffset;

        int cursorStartX = 0;
        int cursorStartY = 0;

        void SetCurrentValue(int value);

        int CurrentValue;
        int Height;
        int Width;

        bool Dragging;
};

#endif // BASESLIDERCOMPONENT_H
