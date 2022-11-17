#include "graphics.h"

const int BoardRenderer::PIECE_COLORS_BREAKPOINTS[PIECE_COLORS_COUNT-1]{4,16,64,256,1024,2048,4096};


int BoardRenderer::get_index_by_value(int value) {
	if (value <= PIECE_COLORS_BREAKPOINTS[0])
		return 0;

	for (int idx = 1; idx < PIECE_COLORS_COUNT-1; ++idx){
		if ((value > PIECE_COLORS_BREAKPOINTS[idx-1]) && (value <= PIECE_COLORS_BREAKPOINTS[idx]))
			return idx;
	}

	return PIECE_COLORS_COUNT-1;
}

PieceColors& BoardRenderer::get_piece_color(int value) {
	return  piece_colors[get_index_by_value(value)];
}


SDL_Surface* BoardRenderer::get_piece_label(int value) {
	if (value < 0)
		value = 0;

	if (!value_labels.count(value)){
		std::string s = std::to_string(value);
		if (value == 0)
			s = "ERROR";
		const char* text = s.c_str();
 		uint32_t tc = get_piece_color(value).text;
 		SDL_Color color{uint8_t((tc>>16)&0xFF), uint8_t((tc>>8)&0xFF), uint8_t((tc>>0)&0xFF), 0xFF};
 		value_labels[value] = TTF_RenderText_Blended(font, text, color);
 		printf("New label added: %s\n", text);
	}

	return value_labels[value];
}


int BoardRenderer::get_padding() {
    return 2;
}

int BoardRenderer::get_border_width() {
    return 1;
}


BoardRenderer::BoardRenderer(SDL_PixelFormat* format) { 
    board = nullptr;

    // colors
    auto f_rgb = [format](uint32_t rgb) -> uint32_t {
        return SDL_MapRGB(format, (rgb>>16)&0xFF, (rgb>>8)&0xFF, rgb&0xFF);
    };

    board_color = f_rgb(0xbababa);
    field_color = f_rgb(0xa3a3a3);

    piece_colors[0] = {f_rgb(0xf5f4d5), f_rgb(0x222222), f_rgb(0x000000)};
    piece_colors[1] = {f_rgb(0xebe2ab), f_rgb(0x222222), f_rgb(0x000000)};
    piece_colors[2] = {f_rgb(0xe3cf8f), f_rgb(0x222222), f_rgb(0x000000)};
    piece_colors[3] = {f_rgb(0xd6ae76), f_rgb(0x222222), f_rgb(0x000000)};
    piece_colors[4] = {f_rgb(0xc78150), f_rgb(0x222222), f_rgb(0x000000)};
    piece_colors[5] = {f_rgb(0xb85332), f_rgb(0x222222), f_rgb(0xFFFFFF)};
    piece_colors[6] = {f_rgb(0xa81818), f_rgb(0x222222), f_rgb(0xFFFFFF)};
    piece_colors[7] = {f_rgb(0x8c075b), f_rgb(0x4d0331), f_rgb(0xFFFFFF)};


}

BoardRenderer::~BoardRenderer() { 
    board = nullptr;
}

void BoardRenderer::attach_board(Board* board) { 
    this->board = board;
}

void BoardRenderer::attach_draw_surface(SDL_Surface* draw_surface) { 
    this->draw_surface = draw_surface;
}

void BoardRenderer::recalc() {
    float display_width = draw_surface->w;
    float display_height = draw_surface->h;
    // TODO offset

    // tile, field, piece sizes
    float tile_size_hor = display_width / Board::COLUMNS;
    float tile_size_ver = display_height / Board::ROWS;
    tile_size = static_cast<int>(std::min<float>(tile_size_hor, tile_size_ver));


    font = TTF_OpenFont("res/Quicksand-Medium.ttf", static_cast<int>(tile_size/2.7f));

    field_size = tile_size - 2*get_padding();
    piece_size = field_size - 2*get_padding();

    board_rect.w = tile_size * Board::COLUMNS;
    board_rect.h = tile_size * Board::ROWS;
    board_rect.x = (display_width - board_rect.w)/2;
    board_rect.y = (display_height - board_rect.h)/2;
    SDL_Log("Tile size %d", tile_size);
    SDL_Log("Board w: %d, h: %d, x: %d, y: %d.", board_rect.w, board_rect.h, board_rect.x, board_rect.y);

    board_surface = SDL_CreateRGBSurface(0, board_rect.w, board_rect.h, 32, 0,0,0,0);
    
}

void BoardRenderer::render() {
    SDL_FillRect(board_surface, NULL, board_color);

    SDL_Rect trect;
    trect.w = field_size;
    trect.h = field_size;

    for(int iy = 0; iy < Board::COLUMNS; ++iy) {
        for(int ix = 0; ix < Board::ROWS; ++ix){
            trect.x = get_padding() + ix * tile_size;
            trect.y = get_padding() + iy * tile_size;
            SDL_FillRect(board_surface, &trect, field_color);
        }
    }

    trect.w = piece_size;
    trect.h = piece_size;
    for(int iy = 0; iy < Board::COLUMNS; ++iy) {
        for(int ix = 0; ix < Board::ROWS; ++ix) {
            int value = board->get_value(ix, iy);
            if (value > 0) {
                PieceColors color = get_piece_color(value);
                trect.x = 2*get_padding() + ix * tile_size;
                trect.y = 2*get_padding() + iy * tile_size;
                SDL_FillRect(board_surface, &trect, color.border);
            }
        }
    }

    trect.w = piece_size-2*get_border_width();
    trect.h = piece_size-2*get_border_width();
    SDL_Rect text_r;
    for(int iy = 0; iy < Board::COLUMNS; ++iy) {
        for(int ix = 0; ix < Board::ROWS; ++ix) {
            int value = board->get_value(ix, iy);
            if (value > 0) {
                PieceColors color = get_piece_color(value);
                trect.x = 2*get_padding()+get_border_width() + ix * tile_size;
                trect.y = 2*get_padding()+get_border_width() + iy * tile_size;
                SDL_FillRect(board_surface, &trect, color.main);
                SDL_Surface* value_text = get_piece_label(value);
//                if (value < 2000)
//                	value_text = value_labels[value];
//                else
//                	value_text = value_labels[0];
                text_r.w = value_text->w;
                text_r.h = value_text->h;
                text_r.x = trect.x + (trect.w - text_r.w)/2;
                text_r.y = trect.y + (trect.h - text_r.h)/2;
                SDL_BlitSurface(value_text, NULL, board_surface, &text_r);
            }
        }
    }


    SDL_BlitSurface(board_surface, NULL, draw_surface, &board_rect);

}
