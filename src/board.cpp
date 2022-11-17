#include "board.h"

/***********************************
 *              BOARD              *
 ***********************************/

int Board::swipe_up() {
	int points = 0;
	for(int ix=0; ix<COLUMNS; ++ix) {
		for(int iy=0; iy<ROWS; ++iy) {
			int value = get_value(ix, iy);
			if (value == 0) {
				for(int im=iy+1; im<ROWS; ++im) {
					int other = get_value(ix, im);
					if (other > 0) {
						set_value(other, ix, iy);
						set_value(0, ix, im);
						break;
					}
				}
				value = get_value(ix, iy);
			}
			// should be executed if value changed
			if (value != 0) {
				for(int im=iy+1; im<ROWS; ++im) {
					int other = get_value(ix, im);
					if (other == value){
						set_value(value+other, ix, iy);
						points += value;
						set_value(0, ix, im);
						break;
					}
				}
			}
		}
	}
	return points;
}

int Board::swipe_down() {
	int points = 0;
	for(int ix=0; ix<COLUMNS; ++ix) {
		for(int iy=ROWS-1; iy>=0; --iy) {
			int value = get_value(ix, iy);
			if (value == 0) {
				for(int im=iy-1; im>=0; --im) {
					int other = get_value(ix, im);
					if (other > 0) {
						set_value(other, ix, iy);
						set_value(0, ix, im);
						break;
					}
				}
				value = get_value(ix, iy);
			}
			// should be executed if value changed
			if (value != 0) {
				for(int im=iy-1; im>=0; --im) {
					int other = get_value(ix, im);
					if (other == value){
						set_value(value+other, ix, iy);
						points += value;
						set_value(0, ix, im);
						break;
					}
				}
			}
		}
	}
	return points;
}


int Board::swipe_left() {
	int points = 0;
	for(int iy=0; iy<ROWS; ++iy) {
		for(int ix=0; ix<COLUMNS; ++ix) {
			int value = get_value(ix, iy);
			if (value == 0) {
				for(int im=ix+1; im<COLUMNS; ++im) {
					int other = get_value(im, iy);
					if (other > 0) {
						set_value(other, ix, iy);
						set_value(0, im, iy);
						break;
					}
				}
				value = get_value(ix, iy);
			}
			// should be executed if value changed
			if (value != 0) {
				for(int im=ix+1; im<COLUMNS; ++im) {
					int other = get_value(im, iy);
					if (other == value) {
						set_value(value+other, ix, iy);
						points += value;
						set_value(0, im, iy);
						break;
					}
				}
			}
		}
	}
	return points;
}

int Board::swipe_right() {
	int points = 0;
	for(int iy=0; iy<ROWS; ++iy) {
		for(int ix=COLUMNS-1; ix>=0; --ix) {
			int value = get_value(ix, iy);
			if (value == 0) {
				for(int im=ix-1; im>=0; --im) {
					int other = get_value(im, iy);
					if (other > 0) {
						set_value(other, ix, iy);
						set_value(0, im, iy);
						break;
					}
				}
				value = get_value(ix, iy);
			}
			// should be executed if value changed
			if (value != 0) {
				for(int im=ix-1; im>=0; --im) {
					int other = get_value(im, iy);
					if (other == value){
						set_value(value+other, ix, iy);
						points += value;
						set_value(0, im, iy);
						break;
					}
				}
			}
		}
	}
	return points;
}

Board::Board() {
    SDL_Log("Creating new board with size %d x %d", Board::COLUMNS, Board::ROWS);
    for(int iy = 0; iy < Board::COLUMNS; ++iy) {
        for(int ix = 0; ix < Board::ROWS; ++ix)
            set_value(0, ix, iy);
    }
    total_points = 0;
    srand(time(NULL));

    // test init
//    set_value(2, 0, 0);
//    set_value(2, 0, 1);
//    set_value(4, 0, 2);
//    set_value(8, 0, 3);
//    set_value(16, 1, 3);
//    set_value(32, 2, 3);
//    set_value(64, 3, 3);
//    set_value(128, 3, 2);
//    set_value(256, 3, 1);
//    set_value(512, 3, 0);
//    set_value(1024, 2, 0);
//    set_value(2048, 1, 0);
//    set_value(2048*2, 1, 1);
//    set_value(2048*4, 1, 2);
//    set_value(2048*8, 2, 2);
//    set_value(2048*16, 2, 1);


        set_value(2, 0, 1);
        set_value(2, 1, 3);
        set_value(2, 1, 1);
        set_value(2, 2, 2);
        set_value(8, 3, 3);
        set_value(16, 0, 3);
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


int Board::count_free_spots() {
	int count = 0;
    for(int iy = 0; iy < Board::COLUMNS; ++iy) {
        for(int ix = 0; ix < Board::ROWS; ++ix)
            if (get_value(ix, iy) == 0)
            	++count;
    }
    return count;
}

void Board::place_random_piece() {
	if(count_free_spots() > 0){
		while(true){
			int x = rand()%COLUMNS;
			int y = rand()%ROWS;
			if (get_value(x, y) == 0){
				int val = 2*(rand()%2 + 1); //todo
				set_value(val, x, y);
				break;
			}
		}
	}
}

void Board::swipe(Direction dir) {
//	SDL_Log("Move: %d", int(dir));
	int points = 0;
	if(dir == Direction::UP)
		points = swipe_up();
	if(dir == Direction::RIGHT)
		points = swipe_right();
	if(dir == Direction::DOWN)
		points = swipe_down();
	if(dir == Direction::LEFT)
		points = swipe_left();

	if (points > 0){
		total_points += points;
		SDL_Log("Got %d points! Total %lu\n", points, total_points);
	}
}
