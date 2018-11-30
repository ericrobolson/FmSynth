#ifndef DRAGGABLECOMPONENT_H
#define DRAGGABLECOMPONENT_H

#include "BaseComponent.h"


class DraggableComponent : public BaseComponent
{
    public:
        DraggableComponent();
        virtual ~DraggableComponent();

        static const int Height = 16;
        static const int Width = 16;

        bool Dragging;
};

#endif // DRAGGABLECOMPONENT_H
