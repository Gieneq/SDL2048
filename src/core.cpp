#include "core.h"

/***********************************
 *               GAME              *
 ***********************************/

Game::Game() {
    SDL_Log("Init game...");
    if(SDL_Init(SDL_INIT_EVERYTHING)) {
    	SDL_LogError(0, "SDL_Init error\n");
        throw std::logic_error(std::string(SDL_GetError()));
    }

     if(TTF_Init()) {
    	 SDL_LogError(0, "SDL_TTF error\n");
         throw std::logic_error(std::string(SDL_GetError()));
     }

    window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, DISPLAY_WIDTH, DISPLAY_HEIGHT, SDL_WINDOW_SHOWN);
    window_surface = SDL_GetWindowSurface(window);

    board = new Board();

    board_renderer = new BoardRenderer(window_surface->format);
    board_renderer->attach_board(board);
    board_renderer->attach_draw_surface(window_surface);
    board_renderer->recalc();
    event = new SDL_Event;


}

Game::~Game() {
    SDL_Log("Finish game...");
    delete event;
	event = nullptr;
    SDL_FreeSurface(window_surface);
    window_surface = nullptr;
    SDL_DestroyWindow(window);
    window = nullptr;
    // TTF_Quit();
    SDL_Quit();
}

void Game::loop() {
    float dt_s = 0.0f;
    uint64_t dt_last_time, dt_current_time;
    uint64_t fps_start_time, fps_end_time;
    uint64_t update_start_time, update_end_time;
    uint64_t render_start_time, render_endtime;
    float fps_time_ms;
    float updating_time_ms;
    float rendering_time_ms;

    while(true) {
        fps_start_time = SDL_GetPerformanceCounter();

        dt_current_time = SDL_GetPerformanceCounter();
        dt_s = (dt_current_time - dt_last_time) / (float)SDL_GetPerformanceFrequency();
        dt_last_time = dt_current_time;

        SDL_PollEvent(event);

        if((event->type == SDL_QUIT) ||
        (event->window.event == SDL_WINDOWEVENT_CLOSE) ||
        (event->type == SDL_KEYUP && event->key.keysym.sym == SDLK_ESCAPE)) {
            break;
        }

        update_start_time = SDL_GetPerformanceCounter();
        update(dt_s);
        update_end_time = SDL_GetPerformanceCounter();
        updating_time_ms = 1000.0f *(update_end_time - update_start_time) / (float)SDL_GetPerformanceFrequency();

        render_start_time = SDL_GetPerformanceCounter();
        render();
        render_endtime = SDL_GetPerformanceCounter();
        rendering_time_ms = 1000.0f *(render_endtime - render_start_time) / (float)SDL_GetPerformanceFrequency();

        fps_end_time = SDL_GetPerformanceCounter();
        // printf("dt: %f s, FPS: %f Hz,\n", dt_s, 1.0f/dt_s);
        fps_time_ms = 1000.0f *(render_endtime - render_start_time) / (float)SDL_GetPerformanceFrequency();
        SDL_Delay(static_cast<uint32_t>(FPS_CAP_INTERVAL_MS - fps_time_ms));

    }
}

void Game::update(float dt) {
    auto ticks = SDL_GetTicks();

    if (event->type == SDL_KEYUP) {
//    	bool move_done = false;
    	if(event->key.keysym.sym == SDLK_UP){
    		board->swipe(Direction::UP);

    	}
    	if(event->key.keysym.sym == SDLK_RIGHT){
    		board->swipe(Direction::RIGHT);
    	}
    	if(event->key.keysym.sym == SDLK_DOWN){
    		board->swipe(Direction::DOWN);
    	}
    	if(event->key.keysym.sym == SDLK_LEFT){
    		board->swipe(Direction::LEFT);
    	}
    	board->place_random_piece();
    }

}

void Game::render() {
    board_renderer->render();
    SDL_UpdateWindowSurface(window);
}
