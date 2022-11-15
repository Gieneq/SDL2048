#include "board.h"

/***********************************
 *              BOARD              *
 ***********************************/

Board::Board() {
    SDL_Log("Creating new board with size %d x %d", Board::COLUMNS, Board::ROWS);
    for(int iy = 0; iy < Board::COLUMNS; ++iy) {
        for(int ix = 0; ix < Board::ROWS; ++ix)
            set_value(0, ix, iy);
    }

    // test init
    set_value(0, 0, 0);
    set_value(2, 0, 1);
    set_value(4, 0, 2);
    set_value(8, 0, 3);
    set_value(16, 1, 3);
    set_value(32, 2, 3);
    set_value(64, 3, 3);
    set_value(128, 3, 2);
    set_value(256, 3, 1);
    set_value(512, 3, 0);
    set_value(1024, 2, 0);
    set_value(2048, 1, 0);
    set_value(2048*2, 1, 1);
    set_value(2048*4, 1, 2);
    set_value(2048*8, 2, 2);
    set_value(2048*16, 2, 1);
}

Board::~Board() {
    SDL_Log("Destructing board.");
}

int Board::get_value(int x, int y) {
    return grid[y][x];
}

void Board::set_value(int value, int x, int y) {
    grid[y][x] = value;
}

bool Board::is_free(int x, int y) {
    return get_value(x,y) > 0;
}
