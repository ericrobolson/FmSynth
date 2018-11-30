#ifndef GAMEWORLD_H
#define GAMEWORLD_H
#include <time.h>

#include "BaseWorld.h"
#include "GraphicsSystem.h"
#include "InputSystem.h"
#include "PositionSystem.h"
#include "SystemTimer.h"

class GameWorld : public BaseWorld
{
    public:
        GameWorld();
        virtual ~GameWorld();
        virtual bool Process();

    private:
        GraphicsSystem _graphicsSystem;
        InputSystem _inputSystem;
        PositionSystem _positionSystem;
        clock_t _cycleClock;
        SystemTimer _systemTimer;
};

#endif // GAMEWORLD_H
