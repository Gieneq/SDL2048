#pragma once
// #include <stdio.h>
#include <SDL2/SDL.h>
 #include <SDL2/SDL_ttf.h>
#include "board.h"
#include <algorithm>
#include <map>
#include <string>


struct PieceColors{
    uint32_t main;
    uint32_t border;
    uint32_t text;
};

//namespace BoardRenderer {
//	const int PIECE_COLORS_BREAKPOINTS[]{4,16,64,256,1024,2048,4096};
//}

class BoardRenderer {
private:
    Board* board;
    SDL_Surface* draw_surface;

    SDL_Rect board_rect;
    SDL_Surface* board_surface;
    uint32_t board_color, field_color;
    static constexpr int PIECE_COLORS_COUNT{8};
    static const int PIECE_COLORS_BREAKPOINTS[PIECE_COLORS_COUNT-1];
    PieceColors piece_colors[PIECE_COLORS_COUNT];

    int get_padding();
    int get_border_width();

    int tile_size;
    int field_size;
    int piece_size;

	TTF_Font* font;
	SDL_Surface* text_1024_surface;
	std::map<int, SDL_Surface*> value_labels;
	int get_index_by_value(int value);
	PieceColors& get_piece_color(int value);
	SDL_Surface* get_piece_label(int value);

public:
    BoardRenderer(SDL_PixelFormat* format);
    ~BoardRenderer();
    void attach_board(Board* board);
    void attach_draw_surface(SDL_Surface* draw_surface);
    void recalc();
    void render();
};
