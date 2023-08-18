#include "../src/sdl2-gdk.hpp"
#include <iostream>

#define IMG_PATH "res/image.png"

int main(int argc, char *argv[]) {    

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("Fatal error initializing SDL: %s\n", SDL_GetError());
    }

    SDL_DisplayMode display;
    SDL_GetCurrentDisplayMode(0, &display);

    SDL_Window *window = SDL_CreateWindow(
        "sdl2-gdk test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280,
        720, SDL_WINDOW_INPUT_FOCUS);

    bool running = true;
    SDL_Event event;

    Scene scene(window, -1, SDL_RENDERER_ACCELERATED);

    Sprite s1(100, 100, 50, 50, IMG_PATH);
    Sprite s2(100, 200, 50, 50, IMG_PATH);
    Sprite s3(100, 300, 50, 50, IMG_PATH);

    scene.Add(s1);
    scene.Add(s2);
    scene.Add(s3);

    // Main game loop
    while (running) {

        scene.Draw();

        while (SDL_PollEvent(&event)) {

            if (event.type == SDL_QUIT) {
                running = false;
            }
        }        
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}