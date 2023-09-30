#ifndef INIT_HPP
#define INIT_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

// Initialize all sdl2-gdk dependencies
void SDL2_GDK_Init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("Fatal error initializing SDL: %s\n", SDL_GetError());
    }

    if (TTF_Init() != 0) {
        printf("Fatal error initializing TTF: %s\n", SDL_GetError());
    }
}

// Quit all sdl2-gdk dependencies
void SDL2_GDK_Quit() {
    SDL_Quit();
    TTF_Quit();
}

#endif