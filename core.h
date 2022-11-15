#pragma once
#include <SDL2/SDL.h>

class Game {
private:
    const char* TITLE = "The Loch Angler";
    const int DISPLAY_WIDTH = 800;
    const int DISPLAY_HEIGHT = 600;
    SDL_Event event;
    SDL_Window* window;
    SDL_Renderer* renderer;
public:
    Game();
    ~Game();
    void loop();
    void update(float dt);
    void render();
};