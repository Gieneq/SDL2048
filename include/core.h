#pragma once
#include <stdio.h>
#include <SDL2/SDL.h>
 #include <SDL2/SDL_ttf.h>
#include "board.h"
#include "graphics.h"
#include <stdexcept>


class Game {
private:
    const char* TITLE = "SDL 2048";
    const int DISPLAY_WIDTH = 2*600;
    const int DISPLAY_HEIGHT = 780;
    const float FPS_CAP_HZ = 30.0f;
    const float FPS_CAP_INTERVAL_MS = 1000.0f / FPS_CAP_HZ;
    SDL_Event* event;
    SDL_Window* window;
    SDL_Surface* window_surface;
    Board* board;
    BoardRenderer* board_renderer;
public:
    Game();
    ~Game();
    void loop();
    void update(float dt);
    void render();
};
