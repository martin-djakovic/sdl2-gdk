#include <SDL2/SDL_image.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>
#include <cstdio>
#include <sdl2-gdk/fonttexture.hpp>

namespace gdk {
void FontTexture::updateTexture() {
  // Cannot create texture if these properties are not set
  if (font == nullptr || renderer == nullptr || text == "") {
    return;
  }

  // Destroy previous texture to prevent memory leak
  SDL_DestroyTexture(sdl_texture);

  surface = TTF_RenderText_Solid(font, text, color);
  sdl_texture = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_FreeSurface(surface);
}

FontTexture::FontTexture() {
  text = "";
  font_path = "";
  font_size = 0;
  color = {255, 255, 255};
}

FontTexture::FontTexture(SDL_Renderer *renderer, const char *text,
                         const char *font_path, unsigned int font_size,
                         SDL_Color color) {
  setText(text);
  setFont(font_path, font_size);
  setColor(color);
  setRenderer(renderer);
  updateTexture();
}

FontTexture::FontTexture(FontTexture *texture) {
  setRenderer(texture->renderer);
  setText(texture->text);
  setColor(texture->color);
  setFont(texture->font_path, texture->font_size);
  updateTexture();
}

FontTexture::FontTexture(FontTexture *texture, const char *text) {
  setRenderer(texture->renderer);
  setText(text);
  setColor(texture->color);
  setFont(texture->font_path, texture->font_size);
  updateTexture();
}

void FontTexture::setText(const char *text) noexcept {
  if (text == "") {
    printf(WARN_COLOR "GDK WARNING:" DEF_COLOR
                      " Set FontTexture text to empty value\n");
  }

  this->text = text;
  updateTexture();
}

void FontTexture::setFont(const char *font_path, unsigned int font_size) {
  // Destroy previous font to prevent memory leak
  TTF_CloseFont(font);

  font = TTF_OpenFont(font_path, font_size);
  this->font_size = font_size;
  this->font_path = font_path;

  // Print error if font file path is invalid
  if (font == NULL) {
    printf(ERR_COLOR "GDK ERROR:" DEF_COLOR " Font with file path" FPATH_COLOR
                     " %s" DEF_COLOR " failed to load\n",
           font_path);
    // Need to set font to nullptr if it is invalid, otherwise TTF_CloseFont()
    // will result in segfault
    font = nullptr;
    return;
  }

  // Print warning if font size is 0
  if (font_size == 0) {
    printf(WARN_COLOR "GDK WARNING:" DEF_COLOR " Font size is 0\n");
  }

  updateTexture();
}

const char *FontTexture::getFontPath() noexcept { return font_path; }

const unsigned int FontTexture::getFontSize() noexcept { return font_size; }

void FontTexture::setColor(SDL_Color color) noexcept {
  this->color = color;
  updateTexture();
}

const SDL_Color FontTexture::getColor() noexcept { return color; }

const bool FontTexture::isLoaded() noexcept {
  bool font_size_invalid = font_size == 0;
  bool texture_invalid = sdl_texture == nullptr;
  bool renderer_invalid = renderer == nullptr;
  bool text_invalid = text == "";
  bool font_invalid = font == nullptr;

  if (font_size_invalid || texture_invalid || renderer_invalid ||
      text_invalid || font_invalid) {
    return false;
  }

  return true;
}
} // namespace gdk
