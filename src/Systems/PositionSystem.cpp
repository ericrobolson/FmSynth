#include <cstddef>

#include "PositionSystem.h"
#include "EntityComponentManager.h"
#include "PositionComponent.h"
#include <mutex>
#include "ThreadPool.h"
#include "GameState.h"
#include "DraggableComponent.h"
#include "InputState.h"

PositionSystem::PositionSystem() : BaseSystem()
{
}

PositionSystem::~PositionSystem()
{
    //dtor
}

void ProcessJob(ECS::EntityComponentManager &ecs, int entityIndex){

    PositionComponent& positionComponent = *ecs.GetComponent<PositionComponent>(entityIndex);

    std::shared_ptr<DraggableComponent> dragPtr = ecs.GetComponent<DraggableComponent>(entityIndex);
    if (dragPtr != nullptr){
        DraggableComponent d = *dragPtr.get();
        if (d.Dragging){
            positionComponent.PositionX = InputState::Instance().CursorX;
            positionComponent.PositionY = InputState::Instance().CursorY;
        }
    }


    /*
    positionComponent.PositionX += movementComponent.HorizontalSpeed;
    positionComponent.PositionY += movementComponent.VerticalSpeed; // Since a positive vspeed means it's moving up, need to reverse it

    // Prevent it from going out of the game window
    int hitboxWidth = 0;
    int hitboxHeight = 0;

    std::shared_ptr<HitboxComponent> hitboxComponentPtr = ecs.GetComponent<HitboxComponent>(entityIndex);
    if (hitboxComponentPtr != nullptr){
        hitboxWidth = hitboxComponentPtr->GetWidth();
        hitboxHeight = hitboxComponentPtr->GetHeight();
    }

    if ((positionComponent.PositionX + hitboxWidth) > GameState::LevelWidth){
        positionComponent.PositionX = GameState::LevelWidth - hitboxWidth;
        movementComponent.HorizontalSpeed = 0;
    }

    if (positionComponent.PositionX < 0){
        positionComponent.PositionX = 0;
        movementComponent.HorizontalSpeed = 0;
    }

    if ((positionComponent.PositionY + hitboxWidth) > GameState::LevelHeight){
        positionComponent.PositionY = GameState::LevelHeight - hitboxHeight;
        movementComponent.VerticalSpeed = 0;
    }

    if (positionComponent.PositionY < 0){
        positionComponent.PositionY = 0;
        movementComponent.VerticalSpeed = 0;
    }

    */
}

bool PositionSystem::Process(ECS::EntityComponentManager &ecs){
    // ignore everything else for not
    std::vector<int> entities = ecs.Search<PositionComponent>();

    std::vector<int>::iterator it;

    for (it = entities.begin(); it != entities.end(); ++it){
        int entityId = *it;

        ThreadPool::Instance().submit([&ecs, entityId](){
                                          ProcessJob(ecs, entityId);
                                      });
    }

    std::future<bool> isDone = ThreadPool::Instance().submit([](){

                                                        return true;
                                                        });
    return isDone.get();
}


