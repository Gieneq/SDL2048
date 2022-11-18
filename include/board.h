#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

enum class Direction {
	UP, RIGHT, DOWN, LEFT, NONE
};

enum class SwipeResponse {
	NONE, EMPTY_MOVE, MERGING_MOVE
};

class Board {
public:
    static const int ROWS = 4;
    static const int COLUMNS = 4;
private:
    int grid[ROWS][COLUMNS]{0};
    int tiles_per_move{1};
    bool last_move_changing;
    int swipe_up();
    int swipe_right();
    int swipe_down();
    int swipe_left();
    void test_pattern();
public:
    unsigned long total_points;
    Board();
    ~Board();
    int get_value(int x, int y);
    void set_value(int value, int x, int y);
    bool is_free(int x, int y);
    SwipeResponse swipe(Direction dir);
    int count_free_spots();
    void place_random_piece();
};
