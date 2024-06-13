#include <errorcolors.hpp>
#include <texture.hpp>

GDK_Texture::GDK_Texture() {}

GDK_Texture::GDK_Texture(SDL_Renderer *renderer) { setRenderer(renderer); }

void GDK_Texture::flagDrawEvent() {}

void GDK_Texture::setRenderer(SDL_Renderer *renderer) {
  this->renderer = renderer;
}

const bool GDK_Texture::isLoaded() noexcept {
  if (sdl_texture == nullptr) {
    return false;
  } else {
    return true;
  }
}

void GDK_Texture::destroy() { SDL_DestroyTexture(sdl_texture); }
