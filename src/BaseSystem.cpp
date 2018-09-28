#include "BaseSystem.h"
#include "EntityComponentManager.h"

BaseSystem::BaseSystem()
{
    //ctor
}

BaseSystem::~BaseSystem()
{
    //dtor
}

bool BaseSystem::Process(ECS::EntityComponentManager &ecs){
    // this is where a system processes entities
    return true;
};
