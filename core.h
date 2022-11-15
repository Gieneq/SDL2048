#pragma once
#include <stdio.h>
#include <SDL2/SDL.h>

class Board {
private:
    static const int ROWS = 4;
    static const int COLUMNS = 4;
    int grid[COLUMNS][ROWS]{0};
public:
    Board();
    ~Board();
    int get_value(int x, int y);
    void set_value(int value, int x, int y);
    bool is_free(int x, int y);
};

class Game {
private:
    const char* TITLE = "SDL 2048";
    const int DISPLAY_WIDTH = 800;
    const int DISPLAY_HEIGHT = 600;
    const float FPS_CAP_HZ = 30.0f;
    const float FPS_CAP_INTERVAL_MS = 1000.0f / FPS_CAP_HZ;
    SDL_Event event;
    SDL_Window* window;
    SDL_Surface* window_surface;
    Board* board;
public:
    Game();
    ~Game();
    void loop();
    void update(float dt);
    void render();
};