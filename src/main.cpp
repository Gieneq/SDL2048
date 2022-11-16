#include <chrono>
#include <iostream>
#include "core.h"

int main(int argc, char* argv[]) {
    std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
    
    Game game;
    game.loop();

    std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::microseconds>(end_time-start_time).count() << " us." << std::endl;
    return 0;
}
