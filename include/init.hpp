#ifndef INIT_HPP
#define INIT_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

// Initialize all sdl2-gdk dependencies
void SDL2_GDK_Init();

// Quit all sdl2-gdk dependencies
void SDL2_GDK_Quit();

#endif