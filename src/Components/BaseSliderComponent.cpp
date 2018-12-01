#include "BaseSliderComponent.h"

BaseSliderComponent::BaseSliderComponent()
{
    xOffset = 20;
    yOffset = 10;
    CurrentValue = 75;

    Height = 100;
    Width = 10;

    Dragging = true;
}

BaseSliderComponent::~BaseSliderComponent()
{
    //dtor
}

void BaseSliderComponent::SetCurrentValue(int value){
    // Boundary handling
    if (value > MaxValue){
        value = MaxValue;
    }

    if (value < MinValue){
        value = MinValue;
    }

    CurrentValue = value;
}
