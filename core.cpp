#include "core.h"


Game::Game() {
    SDL_Log("Init game...");
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, DISPLAY_WIDTH, DISPLAY_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, 0);
}

Game::~Game() {
    SDL_Log("Finish game...");
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

void Game::loop() {
    float dt = 0;
    while(true){
        SDL_PollEvent(&event);
        if((event.type == SDL_QUIT) || (event.window.event == SDL_WINDOWEVENT_CLOSE))
            break;

            
        update(dt);
        render();
    }
}

void Game::update(float dt) {
    auto ticks = SDL_GetTicks();
    SDL_Log("%lu", ticks);

}

void Game::render() {

}