#include <SDL.h>
#include "InputSystem.h"
#include "InputState.h"
#include <string>
#include <iostream>
#include "EntityComponentManager.h"
#include "GameState.h"
#include "ThreadPool.h"
#include <future>
#include <fstream>
#include "SynthAssemblage.h"
#include <memory>
#include "DraggableComponent.h"
#include "PositionComponent.h"

InputSystem::InputSystem() : BaseSystem()
{
    if (SDL_WasInit(SDL_INIT_EVENTS) == 0){
        SDL_Init(SDL_INIT_EVENTS);
    }
    _exit = false;
}

void LeftClickReleaseEvent(ECS::EntityComponentManager &ecs){
    std::vector<int> entities = ecs.Search<DraggableComponent>();

    std::vector<int>::iterator ptr;
    for (ptr = entities.begin(); ptr < entities.end(); ptr++){
        int entityId = *ptr;

        DraggableComponent& d = *ecs.GetComponent<DraggableComponent>(entityId);
        d.Dragging = false;
    }
}

void LeftClickEvent(ECS::EntityComponentManager &ecs){
    std::vector<int> entities = ecs.Search<DraggableComponent>();

    // Get only things that could have been partially clicked on
    entities = ecs.SearchOn<PositionComponent>(entities,
        [](PositionComponent p){
            int cursorX = InputState::Instance().CursorX;
            int cursorY = InputState::Instance().CursorY;

            if (cursorX >= p.PositionX
                && cursorY >= p.PositionY){
                return true;
            }
            return false;
    });

    int cursorX = InputState::Instance().CursorX;
    int cursorY = InputState::Instance().CursorY;

    int draggableWidth = DraggableComponent::Width;
    int draggableHeight = DraggableComponent::Height;

    std::vector<int>::iterator ptr;
    for (ptr = entities.begin(); ptr < entities.end(); ptr++){
        int entityId = *ptr;

        PositionComponent p = *ecs.GetComponent<PositionComponent>(entityId);

        // Check to see if the click was on a draggable position
        if (cursorX <= (p.PositionX + draggableWidth) && cursorY <= (p.PositionY + draggableHeight))
        {
            DraggableComponent& d = *ecs.GetComponent<DraggableComponent>(entityId);
            d.Dragging = true;
        }
    }
}


bool InputSystem::Process(ECS::EntityComponentManager &ecs){
    SDL_Event event;

    // Set the cursor position
    SDL_GetMouseState(&InputState::Instance().CursorX, &InputState::Instance().CursorY);


    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT) {
            InputState::Instance().Exit = true;
            _exit = true;
        }

        SDL_Keycode key = event.key.keysym.sym;

        if (event.type == SDL_KEYDOWN) {
            if (_keyMapper.Escape == key){
                InputState::Instance().Exit = true;
                _exit = true;
            }
        }
         if (event.type == SDL_KEYUP) {
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN){
            switch (event.button.button){
                case SDL_BUTTON_LEFT:
                    InputState::Instance().Button1IsPressed = true;

                    LeftClickEvent(ecs);

                    break;
                case SDL_BUTTON_RIGHT:
                    InputState::Instance().Button2IsPressed = true;
                    SynthAssemblage::Create(ecs, InputState::Instance().CursorX, InputState::Instance().CursorY);
                    break;
            }
        }
        else if (event.type == SDL_MOUSEBUTTONUP){
            switch (event.button.button){
                case SDL_BUTTON_LEFT:
                    InputState::Instance().Button1IsPressed = false;

                    LeftClickReleaseEvent(ecs);
                    break;
                case SDL_BUTTON_RIGHT:
                    InputState::Instance().Button2IsPressed = false;
                    break;
            }
        }
    }

    return !_exit;
}


InputSystem::~InputSystem()
{
    //dtor
}
