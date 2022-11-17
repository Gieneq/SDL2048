# SDL 2048

Attempt to make 2048 game with SDL and C++.

## Worklog
 - [x] Setup stable game loop.
 - [x] Render board.
 - [x] Render pieces with texts.
 - [x] Control and gameplay.
 - [x] Text antyaliasing.
 - [ ] Animation on pieces move.
 - [ ] Animation on new piece popup.
 - [ ] Difficulty levels.
 - [ ] Rounded corners (probably caching).


## Tools
 - [SDL](https://github.com/libsdl-org/SDL/releases)
 - [SDL TTF](https://github.com/libsdl-org/SDL_ttf/releases)


## Project setup on Windows
Comming soon

## Worklog

### Setup board and pieces

[16.11.2022] Added separated abstraction for board and separate rendering.

![2048 game in SDL](img/sdl2048_board_pieces.PNG)


### Gameplay

[17.11.2022] Added controller and new pieces generator. Text antialiasing enabled.

![Pieces merging](img/SDL2048_test_play.gif)

## Reference
 - [SDL documentation](https://www.libsdl.org/release/SDL-1.2.15/docs/)
 - [Timing](https://thenumb.at/cpp-course/sdl2/08/08.html)
 - [lazyfoo SDL tutorial](https://lazyfoo.net/tutorials/SDL/)
 - [Missing libSDL2_ttf-2.0.so.0](https://github.com/CleverRaven/Cataclysm-DDA/issues/30931)

 ```shell
 apt-cache search libsdl2
 sudo apt install libsdl2-2.0-0
 sudo apt install libsdl2-dev
 sudo apt-get install xorg-dev
 
 sudo apt install libsdl2-ttf-2.0-0
 sudo apt-get install libSDL-ttf2.0-0
 sudo apt-get install libSDL-ttf2.0-0:i386

 ```