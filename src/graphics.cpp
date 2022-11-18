#include "graphics.h"

const int BoardRenderer::PIECE_COLORS_BREAKPOINTS[PIECE_COLORS_COUNT - 1] { 2,
		4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048 };

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
//		printf("New label added: %s\n", text);
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

	//main board
	float display_width = draw_surface->w;
	float display_height = draw_surface->h - ctrl_offset;

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

	//top controller
	ctrl_rect.x = board_rect.x;
	ctrl_rect.y = 0;
	ctrl_rect.w = board_rect.w;
	ctrl_rect.h = ctrl_offset;
	ctrl_surface = SDL_CreateRGBSurface(0, ctrl_rect.w, ctrl_rect.h, 32, 0, 0,
			0, 0);

}

void BoardRenderer::index_to_local_coords(int ix, int iy, float &px,
		float &py) {
	px = tile_size / 2.0f + ix * tile_size;
	py = tile_size / 2.0f + iy * tile_size;
}

void BoardRenderer::local_coords_to_index(float px, float py, int &ix,
		int &iy) {
	ix = static_cast<int>((px - tile_size / 2.0f) / tile_size);
	iy = static_cast<int>((py - tile_size / 2.0f) / tile_size);
}

inline float interpolate(float v, float from, float to) {
	return from + v * (to - from);
}

void BoardRenderer::update(float dt) {
	auto it = pieces_animations.begin();
	while (it != pieces_animations.end()) {
		auto anim = *it;
		PieceImage *pi = anim->piece;
		if (pi) {
			if (anim->time_left < 0.0f) {
				pi->cx = anim->e_x;
				pi->cy = anim->e_y;
				pi->scale = anim->e_scale;
				if (anim->post == AnimationPost::MULTIPLY) {
					//emit new
					Piece::Event pe { pi->value * 2, Piece::EventType::CREATE,
							0, 0, -2, -2 };
					local_coords_to_index(pi->cx, pi->cy, pe.start_x,
							pe.start_y);
					board->pieces_events.push(pe);
				}
				// kill both previous
				if ((anim->post == AnimationPost::KILL)
						|| (anim->post == AnimationPost::MULTIPLY))
					pieces_images.erase(
							std::remove(pieces_images.begin(),
									pieces_images.end(), pi),
							pieces_images.end());

				it = pieces_animations.erase(it);
			} else {
				float v = anim->time_left / anim->total_time;
				if (v < 0.0f)
					v = 0.0f;
				if (v > 1.0f)
					v = 1.0f;
				v = 1.0f - v;

				pi->cx = interpolate(v, anim->s_x, anim->e_x);
				pi->cy = interpolate(v, anim->s_y, anim->e_y);
				pi->scale = interpolate(v, anim->s_scale, anim->e_scale);

				anim->time_left -= dt;
				++it;
			}
		}
	}
//	if (pieces_animations.size() <= 0){
//		if (locked){
//			//freeing
//			board->place_random_piece();
//		}
//	}


	locked = pieces_animations.size() > 0;
}

PieceImage* BoardRenderer::get_piece_image(int ix, int iy) {
	SDL_Log("@Looking for: %d x %d:\n", ix, iy);
	for (PieceImage *pi : pieces_images) {
		int pi_ix, pi_iy;
		local_coords_to_index(pi->cx, pi->cy, pi_ix, pi_iy);
		SDL_Log("...See:%f, %f and %d, %d\n", pi->cx, pi->cy, pi_ix, pi_iy);
		if ((pi_ix == ix) && (pi_iy == iy))
			return pi;
	}
	return nullptr;
}

void BoardRenderer::process_event_queue() {
	while (board->pieces_events.size()) {
		Piece::Event piece_event = board->pieces_events.front();
		board->pieces_events.pop();
		SDL_Log("Event (%d) type: %d, from [%d,%d] to [%d,%d],\n",
				piece_event.value, int(piece_event.type), piece_event.start_x,
				piece_event.start_y, piece_event.stop_x, piece_event.stop_y);
		float center_x, center_y;
		index_to_local_coords(piece_event.start_x, piece_event.start_y,
				center_x, center_y);

		switch (piece_event.type) {
		case Piece::EventType::CREATE: {
			PieceImage *pi = new PieceImage { piece_event.value, center_x,
					center_y, float(piece_size), float(piece_size), true };
			pieces_images.push_back(pi);
			Animation *anim = new Animation(pi, 0.0f, INTERVAL, pi->cx, pi->cy,
					0.4f);
			anim->e_scale = 1.0f;
			// after merge
			if (piece_event.stop_x < -1)
				anim->s_scale = 1.2f;
			pieces_animations.push_back(anim);
			break;
		}
		case Piece::EventType::MOVE: {
			PieceImage *pi = get_piece_image(piece_event.start_x,
					piece_event.start_y);
			if (pi == nullptr) {
				SDL_Log("Piece::EventType::MOVE -- no piece image!!!\n\n\n");
				break;
			}
			Animation *anim = new Animation(pi, 0.0f, INTERVAL, pi->cx, pi->cy,
					pi->scale);
			index_to_local_coords(piece_event.stop_x, piece_event.stop_y,
					anim->e_x, anim->e_y);
			pieces_animations.push_back(anim);
			break;
		}
		case Piece::EventType::MERGE: {
			PieceImage *pi_upper = get_piece_image(piece_event.start_x,
					piece_event.start_y);
			PieceImage *pi_lower = get_piece_image(piece_event.stop_x,
					piece_event.stop_y);

			Animation *anim_upper = new Animation(pi_upper, 0.0f, INTERVAL,
					pi_upper->cx, pi_upper->cy, pi_upper->scale);
			index_to_local_coords(piece_event.stop_x, piece_event.stop_y,
					anim_upper->e_x, anim_upper->e_y);
			anim_upper->post = AnimationPost::MULTIPLY;
			pieces_animations.push_back(anim_upper);

			Animation *anim_lower = new Animation(pi_lower, 0.0f, INTERVAL,
					pi_lower->cx, pi_lower->cy, pi_lower->scale);
			anim_lower->post = AnimationPost::KILL;
			pieces_animations.push_back(anim_lower);

			break;
		}
		case Piece::EventType::DELETE: {
			break;
		}
		}
	}

}

void BoardRenderer::render_piece(PieceImage *pi) {
	PieceColors color = get_piece_color(pi->value);
	SDL_Rect rect;
	rect.w = static_cast<int>(pi->w * pi->scale);
	rect.h = static_cast<int>(pi->h * pi->scale);
	rect.x = static_cast<int>(get_padding() + pi->cx - tile_size / 2.0f
			+ (field_size - pi->w * pi->scale) / 2.0f);
	rect.y = static_cast<int>(get_padding() + pi->cy - tile_size / 2.0f
			+ (field_size - pi->h * pi->scale) / 2.0f);
//	rect.x = static_cast<int>(pi->cx - tile_size/2.0f + (field_size-rect.w)/2.0f);
//	rect.y = static_cast<int>(pi->cy - tile_size/2.0f + (field_size-rect.h)/2.0f);
	SDL_FillRect(board_surface, &rect, color.main);

	SDL_Surface *value_text = get_piece_label(pi->value);
	SDL_Rect text_rect;
	text_rect.w = value_text->w;
	text_rect.h = value_text->h;
	text_rect.x = rect.x + (rect.w - text_rect.w) / 2;
	text_rect.y = rect.y + (rect.h - text_rect.h) / 2;
	SDL_BlitSurface(value_text, NULL, board_surface, &text_rect);
}

void BoardRenderer::render() {
	/*****************************************
	 *              CONTROLLER               *
	 *****************************************/
	SDL_FillRect(ctrl_surface, NULL, field_color);
	unsigned long points = board->total_points;
	std::string text = "Points: " + std::to_string(points);

	uint32_t tc = 0xFF000000;
	SDL_Color color { uint8_t((tc >> 16) & 0xFF), uint8_t((tc >> 8) & 0xFF),
			uint8_t((tc >> 0) & 0xFF), 0xFF };

	SDL_Surface *points_surf = TTF_RenderText_Blended(font, text.c_str(),
			color);
	SDL_Rect points_rect;
	points_rect.w = points_surf->w;
	points_rect.h = points_surf->h;
	points_rect.x = 10;
	points_rect.y = (ctrl_rect.h - points_rect.h) / 2;
	SDL_BlitSurface(points_surf, NULL, ctrl_surface, &points_rect);
	SDL_BlitSurface(ctrl_surface, NULL, draw_surface, &ctrl_rect);

	/*****************************************
	 *                 BOARD                 *
	 *****************************************/
	SDL_FillRect(board_surface, NULL, board_color);
	SDL_Rect field_rect;
	field_rect.w = field_size;
	field_rect.h = field_size;

	for (int iy = 0; iy < Board::COLUMNS; ++iy) {
		for (int ix = 0; ix < Board::ROWS; ++ix) {
			field_rect.x = get_padding() + ix * tile_size;
			field_rect.y = get_padding() + iy * tile_size;
			SDL_FillRect(board_surface, &field_rect, field_color);
		}
	}

	for (PieceImage *pi : pieces_images) {
		render_piece(pi);
	}

	SDL_BlitSurface(board_surface, NULL, draw_surface, &board_rect);
}
