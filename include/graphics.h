#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "board.h"
#include <algorithm>
#include <map>
#include <string>
#include <vector>

struct PieceImage {
	int value;
	float cx, cy, w, h;
	bool visibility;
	float scale { 1 };

	PieceImage(int value, float cx, float cy, float w, float h, bool vis) {
		this->value = value;
		this->cx = cx;
		this->cy = cy;
		this->w = w;
		this->h = h;
		this->visibility = vis;
	}
};

enum class AnimationPost {
	NONE, KILL, MULTIPLY
};

struct Animation {
	//easing
	PieceImage *piece;
	float total_time { 1 };
	float delay { 0.25f };
	float time_left { 1 };
	float s_x, s_y;
	float e_x, e_y;
	float s_scale { 1 };
	float e_scale { 1 };
	AnimationPost post { AnimationPost::NONE };
	Animation(PieceImage *piece, float delay, float total_time, float s_x,
			float s_y, float s_scale) {
		this->piece = piece;
		this->delay = delay;
		this->total_time = total_time;
		this->time_left = total_time;
		this->s_x = s_x;
		this->s_y = s_y;
		this->e_x = s_x;
		this->e_y = s_y;
		this->s_scale = s_scale;
		this->e_scale = s_scale;
	}
};

struct PieceColors {
	uint32_t main;
	uint32_t secondary;
	uint32_t text;
};

class BoardRenderer {
private:
	Board *board;
	SDL_Surface *draw_surface;

	const float INTERVAL = 0.2;

	SDL_Rect ctrl_rect;
	SDL_Surface *ctrl_surface;
	SDL_Rect board_rect;
	SDL_Surface *board_surface;
	uint32_t board_color, field_color;
	static constexpr int PIECE_COLORS_COUNT { 12 };
	static const int PIECE_COLORS_BREAKPOINTS[PIECE_COLORS_COUNT - 1];
	PieceColors piece_colors[PIECE_COLORS_COUNT];

	int get_padding();

	int tile_size;
	int field_size;
	int piece_size;

	TTF_Font *font;
	SDL_PixelFormat *format;
	SDL_Surface *text_1024_surface;
	std::map<int, SDL_Surface*> value_labels;
	int get_index_by_value(int value);
	PieceColors& get_piece_color(int value);
	SDL_Surface* get_piece_label(int value);
	std::vector<PieceImage*> pieces_images;
	std::vector<Animation*> pieces_animations;
	void render_piece(PieceImage *pi);

	void index_to_local_coords(int ix, int iy, float &px, float &py);
	void local_coords_to_index(float px, float py, int &ix, int &iy);
public:
	BoardRenderer(SDL_PixelFormat *format);
	~BoardRenderer();
	void attach_board(Board *board);
	void attach_draw_surface(SDL_Surface *draw_surface);
	void recalc();
	void process_event_queue();
	void update(float dt);
	void render();
	PieceImage* get_piece_image(int ix, int iy);
	bool locked;
};
