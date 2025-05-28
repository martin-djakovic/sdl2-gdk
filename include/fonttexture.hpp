#ifndef FONT_TEXTURE_HPP
#define FONT_TEXTURE_HPP

#ifdef __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#elif _WIN32
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#elif __APPLE__
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#else
#error "FATAL ERROR: Operating system not supported"

#endif

#include <errorcolors.hpp>
#include <texture.hpp>

namespace gdk {
class FontTexture : public Texture {
private:
  const char *text;
  const char *font_path;
  unsigned int font_size;
  TTF_Font *font = nullptr;
  SDL_Color color;
  SDL_Surface *surface;

  /**
   * @brief Creates/updates texture based on FontTexture properties
   */
  void updateTexture();

public:
  /**
   * @brief Doesn't set font or text. Sets text color to white
   */
  FontTexture();

  /**
   * @brief Set font texture properties
   */
  FontTexture(SDL_Renderer *renderer, const char *text, const char *font_path,
              unsigned int font_size, SDL_Color color);

  FontTexture(FontTexture *texture);

  FontTexture(FontTexture *texture, const char *text);

  void setText(const char *text) noexcept;

  /**
   * @brief Set text font
   * @param font_path relative/absolute filesystem path to the font file (.otf,
   * .ttf)
   */
  void setFont(const char *font_path, unsigned int font_size);

  /**
   * @brief Get file path to the font file
   */
  const char *getFontPath() noexcept;

  const unsigned int getFontSize() noexcept;

  /**
   * @brief Sets text color
   * @param color RGBA color formatted as {R,G,B,A}*/
  void setColor(SDL_Color color) noexcept;

  const SDL_Color getColor() noexcept;

  /**
   * @brief Determines if GDK_FontTexture is ready to be drawn (font, font size,
   * renderer and text are set and have valid values)
   * @return true if texture is ready to be drawn
   * @false if texture cannot be dawn
   */
  const bool isLoaded() noexcept;
};
} // namespace gdk

#endif
