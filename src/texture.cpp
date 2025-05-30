#include <sdl2-gdk/errorcolors.h>
#include <sdl2-gdk/texture.hpp>

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
