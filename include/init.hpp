#ifndef INIT_HPP
#define INIT_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

/*
 * @brief Initialize all sdl2-gdk dependencies
 */
void gdkInit();

/*
 * @brief Quit all sdl2-gdk dependencies
 */
void gdkQuit();

#endif
