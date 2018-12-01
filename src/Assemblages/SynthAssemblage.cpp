#include "SynthAssemblage.h"
#include "EntityComponentManager.h"
#include <memory>
#include "SdlRectangleComponent.h"
#include "PositionComponent.h"
#include "VisibleComponent.h"
#include "DraggableComponent.h"
#include "BaseSliderComponent.h"

SynthAssemblage::SynthAssemblage()
{
    //ctor
}

SynthAssemblage::~SynthAssemblage()
{
    //dtor
}


std::shared_ptr<int> SynthAssemblage::Create(ECS::EntityComponentManager &ecs, int x, int y){
    std::shared_ptr<int> entityId = ecs.AddEntity();

    if (entityId != nullptr){
        int id = *entityId.get();

        PositionComponent& position = ecs.AddComponent<PositionComponent>(id);
        position.PositionX = x;
        position.PositionY = y;

        SdlRectangleComponent& boundingRectangle = ecs.AddComponent<SdlRectangleComponent>(id);

        boundingRectangle.Height = 256;
        boundingRectangle.Width = 512;


        ecs.AddComponent<VisibleComponent>(id);
        ecs.AddComponent<DraggableComponent>(id);

        ecs.AddComponent<BaseSliderComponent>(id);
    }

    return entityId;


}
