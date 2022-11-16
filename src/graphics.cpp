#include "graphics.h"

PieceColors& BoardRenderer::get_piece_color(int value) {
    if (value <= 4)
        return piece_colors[0];
    else if (value <= 16)
        return piece_colors[1];
    else if (value <= 64)
        return piece_colors[2];
    else if (value <= 256)
        return piece_colors[3];
    else if (value <= 1024)
        return piece_colors[4];
    else if (value <= 2048)
        return piece_colors[5];
    else if (value <= 4096)
        return piece_colors[6];
    else
        return piece_colors[7];
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


    piece_colors[0] = {f_rgb(0xf5f4d5), f_rgb(0x222222), f_rgb(0x999999)};
    piece_colors[1] = {f_rgb(0xebe2ab), f_rgb(0x222222), f_rgb(0x999999)};
    piece_colors[2] = {f_rgb(0xe3cf8f), f_rgb(0x222222), f_rgb(0x999999)};
    piece_colors[3] = {f_rgb(0xd6ae76), f_rgb(0x222222), f_rgb(0xAAAAAA)};
    piece_colors[4] = {f_rgb(0xc78150), f_rgb(0x222222), f_rgb(0xDDDDDD)};
    piece_colors[5] = {f_rgb(0xb85332), f_rgb(0x222222), f_rgb(0xEEEEEE)};
    piece_colors[6] = {f_rgb(0xa81818), f_rgb(0x222222), f_rgb(0xEFEFEF)};
    piece_colors[7] = {f_rgb(0x8c075b), f_rgb(0x4d0331), f_rgb(0xFFFFFF)};

     font = TTF_OpenFont("res/Quicksand-Bold.ttf", 32);
//     int ww, hh;
//     TTF_SizeText(font, "1024", &ww, &hh);
//     printf("%d, %d\n", ww, hh);

     value_labels[0] = TTF_RenderText_Solid(font, "ERROR", {0x1, 0x11, 0x11, 0xff});
     value_labels[2] = TTF_RenderText_Solid(font, "2", {0x1, 0x11, 0x11, 0xff});
     value_labels[4] = TTF_RenderText_Solid(font, "4", {0x1, 0x11, 0x11, 0xff});
     value_labels[8] = TTF_RenderText_Solid(font, "8", {0x1, 0x11, 0x11, 0xff});
     value_labels[16] = TTF_RenderText_Solid(font, "16", {0x1, 0x11, 0x11, 0xff});
     value_labels[32] = TTF_RenderText_Solid(font, "32", {0x1, 0x11, 0x11, 0xff});
     value_labels[64] = TTF_RenderText_Solid(font, "64", {0x1, 0x11, 0x11, 0xff});
     value_labels[128] = TTF_RenderText_Solid(font, "128", {0x1, 0x11, 0x11, 0xff});
     value_labels[256] = TTF_RenderText_Solid(font, "256", {0x1, 0x11, 0x11, 0xff});
     value_labels[512] = TTF_RenderText_Solid(font, "512", {0x1, 0x11, 0x11, 0xff});
     value_labels[1024] = TTF_RenderText_Solid(font, "1024", {0x1, 0x11, 0x11, 0xff});
     value_labels[2048] = TTF_RenderText_Solid(font, "2048", {0x1, 0x11, 0x11, 0xff});


//     text_1024_surface = TTF_RenderText_Solid(font, "1024", {0xff, 0, 0, 0xff});
//     printf("%p\n", text_1024_surface);
//     printf("ttf: %d, %d", text_1024_surface->w, text_1024_surface->h);
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
                SDL_Surface* value_text = nullptr;
                if (value < 2000)
                	value_text = value_labels[value];
                else
                	value_text = value_labels[0];
                text_r.w = value_text->w;
                text_r.h = value_text->h;
                text_r.x = trect.x + (trect.w - text_r.w)/2;
                text_r.y = trect.y + (trect.h - text_r.h)/2;
                SDL_BlitSurface(value_text, NULL, board_surface, &text_r);
            }
        }
    }



//    SDL_BlitSurface(text_1024_surface, NULL, board_surface, NULL);
    SDL_BlitSurface(board_surface, NULL, draw_surface, &board_rect);

}
