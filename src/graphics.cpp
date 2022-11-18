#include "graphics.h"

const int BoardRenderer::PIECE_COLORS_BREAKPOINTS[PIECE_COLORS_COUNT - 1] { 2, 4, 8,
		16, 32, 64, 128, 256, 512, 1024, 2048 };

static void use_pieces_collors(SDL_PixelFormat *format,
		PieceColors *piece_colors, uint32_t &board_color,
		uint32_t &field_color) {
	// colors
	auto f_rgb = [format](uint32_t rgb) -> uint32_t {
		return SDL_MapRGB(format, (rgb >> 16) & 0xFF, (rgb >> 8) & 0xFF,
				rgb & 0xFF);
	};

	board_color = f_rgb(0xbababa);
	field_color = f_rgb(0xa3a3a3);

	//                main,   border,   text
	int idx = 0;
	piece_colors[idx++] = { f_rgb(0xedecda), f_rgb(0xcfcdb2), f_rgb(0x000000) }; //white
	piece_colors[idx++] = { f_rgb(0xebcc34), f_rgb(0xd6b929), f_rgb(0x000000) }; //yellow
	piece_colors[idx++] = { f_rgb(0x45bacc), f_rgb(0x2ea5b8), f_rgb(0x000000) }; //aqua
	piece_colors[idx++] = { f_rgb(0xd16be8), f_rgb(0x7c3bb3), f_rgb(0x000000) }; //pink
	piece_colors[idx++] = { f_rgb(0xe08938), f_rgb(0xd47e2f), f_rgb(0x000000) }; //orange

	piece_colors[idx++] = { f_rgb(0x8ed439), f_rgb(0x7dbf2c), f_rgb(0x000000) }; //light_green
	piece_colors[idx++] = { f_rgb(0xcf0c19), f_rgb(0x7d070f), f_rgb(0x000000) }; //red
	piece_colors[idx++] = { f_rgb(0x451fed), f_rgb(0x2606b8), f_rgb(0xFFFFFF) }; //ultramaryn
	piece_colors[idx++] = { f_rgb(0x0a6e2b), f_rgb(0x044a1b), f_rgb(0xFFFFFF) }; //green
	piece_colors[idx++] = { f_rgb(0xad2183), f_rgb(0x8a1366), f_rgb(0xFFFFFF) }; //purple
	piece_colors[idx++] = { f_rgb(0x601870), f_rgb(0x470e54), f_rgb(0xFFFFFF) }; //dark_purple

	piece_colors[idx++] = { f_rgb(0x230c38), f_rgb(0x11051c), f_rgb(0xFFFFFF) }; //black

}

int BoardRenderer::get_index_by_value(int value) {
	if (value <= PIECE_COLORS_BREAKPOINTS[0])
		return 0;

	for (int idx = 1; idx < PIECE_COLORS_COUNT - 1; ++idx) {
		if ((value > PIECE_COLORS_BREAKPOINTS[idx - 1])
				&& (value <= PIECE_COLORS_BREAKPOINTS[idx]))
			return idx;
	}

	return PIECE_COLORS_COUNT - 1;
}

PieceColors& BoardRenderer::get_piece_color(int value) {
	return piece_colors[get_index_by_value(value)];
}

SDL_Surface* BoardRenderer::get_piece_label(int value) {
	if (value < 0)
		value = 0;

	if (!value_labels.count(value)) {
		std::string s = std::to_string(value);
		if (value == 0)
			s = "ERROR";
		const char *text = s.c_str();
		uint32_t tc = get_piece_color(value).text;
		SDL_Color color { uint8_t((tc >> 16) & 0xFF), uint8_t((tc >> 8) & 0xFF),
				uint8_t((tc >> 0) & 0xFF), 0xFF };
		value_labels[value] = TTF_RenderText_Blended(font, text, color);
		printf("New label added: %s\n", text);
	}

	return value_labels[value];
}

int BoardRenderer::get_padding() {
	return 6;
}

BoardRenderer::BoardRenderer(SDL_PixelFormat *format) {
	board = nullptr;
	this->format = format;
	use_pieces_collors(this->format, piece_colors, board_color, field_color);
	draw_surface = nullptr;
	board_surface = nullptr;
	text_1024_surface = nullptr;
	font = nullptr;
	tile_size = 100;
	field_size = 90;
	piece_size = 80;
}

BoardRenderer::~BoardRenderer() {
	board = nullptr;
}

void BoardRenderer::attach_board(Board *board) {
	this->board = board;
}

void BoardRenderer::attach_draw_surface(SDL_Surface *draw_surface) {
	this->draw_surface = draw_surface;
}

void BoardRenderer::recalc() {
	//top controller
	static const int MIN_CONTROLLER_HEIGHT = 100;
	int ctrl_offset = int(draw_surface->h * 0.12);
//	if (ctrl_offset < MIN_CONTROLLER_HEIGHT)
//		ctrl_offset = MIN_CONTROLLER_HEIGHT;



	//main board
	float display_width = draw_surface->w;
	float display_height = draw_surface->h - ctrl_offset;

	// tile, field, piece sizes
	float tile_size_hor = display_width / Board::COLUMNS;
	float tile_size_ver = display_height / Board::ROWS;
	tile_size = static_cast<int>(std::min<float>(tile_size_hor, tile_size_ver));

	font = TTF_OpenFont("res/Quicksand-Medium.ttf",
			static_cast<int>(tile_size / 2.7f));

	field_size = tile_size - 2 * get_padding();
	piece_size = field_size;

	board_rect.w = tile_size * Board::COLUMNS;
	board_rect.h = tile_size * Board::ROWS;
	board_rect.x = (display_width - board_rect.w) / 2;
	board_rect.y = (display_height - board_rect.h) / 2 + ctrl_offset;
	SDL_Log("Tile size %d", tile_size);
	SDL_Log("Board w: %d, h: %d, x: %d, y: %d.", board_rect.w, board_rect.h,
			board_rect.x, board_rect.y);

	board_surface = SDL_CreateRGBSurface(0, board_rect.w, board_rect.h, 32, 0,
			0, 0, 0);


	ctrl_rect.x = board_rect.x;
	ctrl_rect.y = 0;
	ctrl_rect.w = board_rect.w;
	ctrl_rect.h = ctrl_offset;
	ctrl_surface = SDL_CreateRGBSurface(0, ctrl_rect.w, ctrl_rect.h, 32, 0,
			0, 0, 0);

}

void BoardRenderer::render() {
	SDL_FillRect(board_surface, NULL, board_color);

	// conttroller
	SDL_FillRect(ctrl_surface, NULL, field_color);
	unsigned long points = board->total_points;
	std::string text = "Points: " + std::to_string(points);

	uint32_t tc = 0xFF000000;
	SDL_Color color { uint8_t((tc >> 16) & 0xFF), uint8_t((tc >> 8) & 0xFF),
					uint8_t((tc >> 0) & 0xFF), 0xFF };

	SDL_Surface* points_surf = TTF_RenderText_Blended(font, text.c_str(), color);
	SDL_Rect points_rect;
	points_rect.w = points_surf->w;
	points_rect.h = points_surf->h;
	points_rect.x = 10;
	points_rect.y = (ctrl_rect.h - points_rect.h) / 2;
	SDL_BlitSurface(points_surf, NULL, ctrl_surface, &points_rect);



	//board
	SDL_Rect piece_rect;
	piece_rect.w = field_size;
	piece_rect.h = field_size;

	for (int iy = 0; iy < Board::COLUMNS; ++iy) {
		for (int ix = 0; ix < Board::ROWS; ++ix) {
			piece_rect.x = get_padding() + ix * tile_size;
			piece_rect.y = get_padding() + iy * tile_size;
			SDL_FillRect(board_surface, &piece_rect, field_color);
		}
	}

	piece_rect.w = piece_size;
	piece_rect.h = piece_size;
	SDL_Rect text_rect;
	for (int iy = 0; iy < Board::COLUMNS; ++iy) {
		for (int ix = 0; ix < Board::ROWS; ++ix) {
			int value = board->get_value(ix, iy);
			// texts
			if (value > 0) {
				PieceColors color = get_piece_color(value);
				piece_rect.x = get_padding() + ix * tile_size;
				piece_rect.y = get_padding() + iy * tile_size;
				SDL_FillRect(board_surface, &piece_rect, color.main);
				SDL_Surface *value_text = get_piece_label(value);
				text_rect.w = value_text->w;
				text_rect.h = value_text->h;
				text_rect.x = piece_rect.x + (piece_rect.w - text_rect.w) / 2;
				text_rect.y = piece_rect.y + (piece_rect.h - text_rect.h) / 2;
				SDL_BlitSurface(value_text, NULL, board_surface, &text_rect);
			}
		}
	}

	SDL_BlitSurface(ctrl_surface, NULL, draw_surface, &ctrl_rect);
	SDL_BlitSurface(board_surface, NULL, draw_surface, &board_rect);

}
