#pragma once
#include <stdio.h>
#include <SDL2/SDL.h>

class Board {
public:
    static const int ROWS = 4;
    static const int COLUMNS = 4;
private:
    int grid[ROWS][COLUMNS]{0};
public:
    Board();
    ~Board();
    int get_value(int x, int y);
    void set_value(int value, int x, int y);
    bool is_free(int x, int y);
};
