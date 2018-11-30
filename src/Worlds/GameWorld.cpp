#include "GameWorld.h"
#include "InputState.h"
#include <stdio.h>
#include <time.h>
#include <thread>
#include <memory>

#include "GameState.h"
#include "SystemTimer.h"

#include <iostream>

GameWorld::GameWorld() : BaseWorld()
{
    _cycleClock = clock();
}

GameWorld::~GameWorld()
{
    //dtor
}

bool GameWorld::Process(){
    _inputSystem.Process(entityComponentManager);

    _positionSystem.Process(entityComponentManager);

    _graphicsSystem.Process(entityComponentManager);

    if (InputState::Instance().Exit == true){
        return false;
    }

    return true;
}
