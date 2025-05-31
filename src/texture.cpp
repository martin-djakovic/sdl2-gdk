#ifdef __linux__
#include <sdl2-gdk/errorcolors.h>
#include <sdl2-gdk/texture.hpp>

#elif _WIN32
#include <errorcolors.h>
#include <texture.hpp>

#elif __APPLE__
#include <errorcolors.h>
#include <texture.hpp>

#else
#error "FATAL ERROR: Operating system not supported"

#endif

namespace gdk {
Texture::Texture() {}

Texture::Texture(SDL_Renderer *renderer) { setRenderer(renderer); }

void Texture::flagDrawEvent() {}

void Texture::setRenderer(SDL_Renderer *renderer) { this->renderer = renderer; }

const bool Texture::isLoaded() noexcept {
  if (sdl_texture == nullptr) {
    return false;
  } else {
    return true;
  }
}

void Texture::destroy() { SDL_DestroyTexture(sdl_texture); }
} // namespace gdk
