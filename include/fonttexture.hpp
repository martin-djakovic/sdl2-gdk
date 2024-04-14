#ifndef FONT_TEXTURE_HPP
#define FONT_TEXTURE_HPP

#include "texture.hpp"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <errorcolors.hpp>

class GDK_FontTexture {
  friend class GDK_Sprite;

private:
  const char *text;
  const char *font_path;
  unsigned int font_size;
  TTF_Font *font = nullptr;
  SDL_Color color;
  SDL_Texture *sdl_texture = nullptr;
  SDL_Surface *surface;
  SDL_Renderer *renderer = nullptr;

  /*
   * @brief Creates/updates texture based on GDK_FontTexture properties
   */
  void updateTexture();

public:
  /*
   * @brief Doesn't set font or text. Sets text color to white
   */
  GDK_FontTexture();

  /*
   * @brief Set font texture properties
   */
  GDK_FontTexture(SDL_Renderer *renderer, const char *text,
                  const char *font_path, unsigned int font_size,
                  SDL_Color color);

  /*
   * @brief Set renderer on which texture will be created and draw on
   */
  void setRenderer(SDL_Renderer *renderer);

  /*
   * @brief Free texture object from memory
   */
  void destroy();

  void setText(const char *text) noexcept;

  /*
   * @brief Set text font
   * @param font_path relative/absolute filesystem path to the font file (.otf,
   * .ttf)
   */
  void setFont(const char *font_path, unsigned int font_size);

  /*
   * @brief Get file path to the font file
   */
  const char *getFontPath() noexcept;

  const unsigned int getFontSize() noexcept;

  /*
   * @brief Sets text color
   * @param color RGBA color formatted as {R,G,B,A}*/
  void setColor(SDL_Color color) noexcept;

  const SDL_Color getColor() noexcept;

  /*
   * @brief Determines if GDK_FontTexture is ready to be drawn (font, font size,
   * renderer and text are set and have valid values)
   * @return true if texture is ready to be drawn
   * @false if texture cannot be dawn
   */
  const bool isLoaded() noexcept;
};

#endif
