#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <time.h>
#include <SDL2/SDL.h>

enum class Direction {
	UP, RIGHT, DOWN, LEFT, NONE
};

enum class SwipeResponse {
	NONE, EMPTY_MOVE, MERGING_MOVE
};

namespace Piece {
enum class EventType {
	CREATE, MOVE, MERGE, DELETE
};

struct Event {
	int value;
	EventType type;
	int start_x;
	int start_y;
	int stop_x;
	int stop_y;

	Event(int value, EventType type, int start_x, int start_y, int stop_x,
			int stop_y) {
		this->value = value;
		this->type = type;
		this->start_x = start_x;
		this->start_y = start_y;
		this->stop_x = stop_x;
		this->stop_y = stop_y;
	}
};
}

class Board {
public:
	static const int ROWS = 4;
	static const int COLUMNS = 4;
private:
	int grid[ROWS][COLUMNS] { 0 };
	int tiles_per_move { 1 };
	bool last_move_changing;
	int swipe_up();
	int swipe_right();
	int swipe_down();
	int swipe_left();
	void test_pattern();
public:
	std::queue<Piece::Event> pieces_events;
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
