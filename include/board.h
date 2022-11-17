#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

enum class Direction {
	UP, RIGHT, DOWN, LEFT
};

class Board {
public:
    static const int ROWS = 4;
    static const int COLUMNS = 4;
private:
    int grid[ROWS][COLUMNS]{0};
    unsigned long total_points;
    int swipe_up();
    int swipe_right();
    int swipe_down();
    int swipe_left();
public:
    Board();
    ~Board();
    int get_value(int x, int y);
    void set_value(int value, int x, int y);
    bool is_free(int x, int y);
    void swipe(Direction dir);
    int count_free_spots();
    void place_random_piece();
};
