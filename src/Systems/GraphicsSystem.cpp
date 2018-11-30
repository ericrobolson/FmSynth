#include <SDL.h>
#include <list>
#include "GraphicsSystem.h"
#include "EntityComponentManager.h"
#include "PositionComponent.h"
#include <math.h>
#include <future>
#include "ThreadPool.h"
#include "InputState.h"
#include <mutex>
#include "GameState.h"
#include "VisibleComponent.h"
#include "SdlRectangleComponent.h"
#include "DraggableComponent.h"

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;
const int SCREEN_BITSPERPIXEL = 32;

int ScaleGraphics(int value){
    return GameState::GfxScaling * value;
}

// Initialize SDL
GraphicsSystem::GraphicsSystem() : BaseSystem()
{
    std::unique_lock<std::mutex> lock(_resourceMutex);
    if (SDL_WasInit(SDL_INIT_VIDEO) == 0){
        SDL_Init(SDL_INIT_VIDEO);
    }

    // Possibly move the renderer & window out to a static object, so there's only one? but then, why would we need more than one graphics system?
    _window = nullptr;
    _renderer = nullptr;

    _window = SDL_CreateWindow( "Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}



void GraphicsSystem::ProcessJob(ECS::EntityComponentManager &ecs, int entityId){
    std::unique_lock<std::mutex> lock(_resourceMutex);

    std::shared_ptr<PositionComponent> positionPtr = ecs.GetComponent<PositionComponent>(entityId);

    if (positionPtr == nullptr){
        return;
    }

    PositionComponent position = *positionPtr.get();

    std::shared_ptr<SdlRectangleComponent> rectanglePtr = ecs.GetComponent<SdlRectangleComponent>(entityId);
    if (rectanglePtr != nullptr){
        SdlRectangleComponent rectangle = *rectanglePtr.get();

        SDL_SetRenderDrawColor(_renderer, rectangle.Color.Red, rectangle.Color.Green, rectangle.Color.Blue, rectangle.Color.Alpha);

        SDL_Rect sdlRect;

        sdlRect.x = position.PositionX;
        sdlRect.y = position.PositionY;
        sdlRect.w = rectangle.Width;
        sdlRect.h = rectangle.Height;

        SDL_RenderDrawRect(_renderer, &sdlRect);
    }

    std::shared_ptr<DraggableComponent> draggablePtr = ecs.GetComponent<DraggableComponent>(entityId);
    if (draggablePtr != nullptr){
        DraggableComponent dragComponent = *draggablePtr.get();

        SDL_SetRenderDrawColor(_renderer, 144, 145, 142, SDL_ALPHA_OPAQUE);

        SDL_Rect sdlRect;

        sdlRect.x = position.PositionX;
        sdlRect.y = position.PositionY;
        sdlRect.w = dragComponent.Width;
        sdlRect.h = dragComponent.Height;

        SDL_RenderDrawRect(_renderer, &sdlRect);

    }

/*
    // Draw Hitboxes
    if (DrawHitboxes){
        std::shared_ptr<HitboxComponent> rectanglePtr = ecs.GetComponent<HitboxComponent>(entityId);
        if (rectanglePtr != nullptr && positionPtr != nullptr){
            SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);

            HitboxComponent rectangle = *rectanglePtr.get();
            PositionComponent position = *positionPtr.get();

            SDL_Rect sdlRect;

            sdlRect.x = ScaleGraphics(position.PositionX);
            sdlRect.y = ScaleGraphics(position.PositionY);
            sdlRect.w = ScaleGraphics(rectangle.GetWidth());
            sdlRect.h = ScaleGraphics(rectangle.GetHeight());

            SDL_RenderDrawRect(_renderer, &sdlRect);
        }
    }

    // Draw Deck/cards
    std::shared_ptr<DeckComponent> deckPtr = ecs.GetComponent<DeckComponent>(entityId);
    if (deckPtr != nullptr){
        DeckComponent deck = *deckPtr.get();

        // Draw energy
        const int energyX = 50;
        const int energyY = 500;
        const int energyW = 16;

        for (int i = 0; i < deck.GetEnergy(); i++){
            SDL_SetRenderDrawColor(_renderer, 0, 0, 255, 255);
            SDL_Rect sdlRect;

            int offset = i * (energyW + 2); // offset each energy rectangle for each energy, with a spacing of 2

            sdlRect.x = ScaleGraphics(energyX + offset);
            sdlRect.y = ScaleGraphics(energyY);
            sdlRect.w = ScaleGraphics(energyW);
            sdlRect.h = ScaleGraphics(energyW);

            SDL_RenderFillRect(_renderer, &sdlRect);
        }

        // Draw Cards
         // Draw energy
        const int cardX = 150;
        const int cardY = 500;
        const int cardW = 16;

        for (int i = 0; i < deck.GetHand().size(); i++){

            if (deck.GetActiveCardIndex() == i){
                SDL_SetRenderDrawColor(_renderer, 0, 255, 255, 255);
            }
            else{
                SDL_SetRenderDrawColor(_renderer, 0, 255, 0, 255);
            }

            SDL_Rect sdlRect;

            int offset = i * (cardW + 2); // offset each energy rectangle for each energy, with a spacing of 2

            sdlRect.x = ScaleGraphics(cardX + offset);
            sdlRect.y = ScaleGraphics(cardY);
            sdlRect.w = ScaleGraphics(cardW);
            sdlRect.h = ScaleGraphics(cardW * 2);

            SDL_RenderFillRect(_renderer, &sdlRect);
        }
    }

    */
}

bool GraphicsSystem::Process(ECS::EntityComponentManager &ecs){
    std::unique_lock<std::mutex> lock(_resourceMutex);

    SDL_SetRenderDrawColor(_renderer, 34,139,34, 255);  // Dark green.
    SDL_RenderClear(_renderer);

    lock.unlock();

    std::vector<int> entityIds = ecs.Search<VisibleComponent>(); // change to visible component?

    while (entityIds.empty() == false){
        int entityId = entityIds.back();
        entityIds.pop_back();

        ThreadPool::Instance().submit([this, &ecs, entityId](){
                                            ProcessJob(ecs, entityId);
                                        });
    }

    std::future<bool> isDone = ThreadPool::Instance().submit([](){
                                                        return true;
                                                        });
    isDone.get();

    // Swap buffers.
    lock.lock();
    SDL_RenderPresent(_renderer);

    return true;
}

GraphicsSystem::~GraphicsSystem()
{
    std::unique_lock<std::mutex> lock(_resourceMutex);
    // Cleanup SDL
    if (_renderer != nullptr){
        SDL_DestroyRenderer(_renderer);
    }

    if (_window != nullptr){
        SDL_DestroyWindow(_window);
        _window = nullptr;
    }
}
