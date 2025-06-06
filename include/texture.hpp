#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#ifdef __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <sdl2-gdk/apiprefix.h>

#elif _WIN32
#include <SDL.h>
#include <SDL_image.h>
#include <apiprefix.h>

#elif __APPLE__
#include <SDL.h>
#include <SDL_image.h>
#include <apiprefix.h>

#else
#error "FATAL ERROR: Operating system not supported"

#endif

namespace gdk {
class API_PREFIX Texture {
  friend class Sprite;
  friend class Scene;

protected:
  SDL_Texture *sdl_texture = nullptr;
  SDL_Renderer *renderer = nullptr;

  /**
   * @brief Gets called every time a sprite draws a texture. Not implemented in
   * GDK_Texture, but children of GDK_Texture can use it as a signal to
   * update/change (e.g. GDK_AnimatedTexture)
   */
  virtual void flagDrawEvent();

public:
  Texture();
  /**
   * @param renderer renderer on which texture will be created and drawn on
   */
  Texture(SDL_Renderer *renderer);

  /**
   * @brief Sets renderer on which texture will be created and drawn. Must be
   * the same renderer that is being used in the containing scene
   *
   * @param renderer renderer to create texture on
   */
  void setRenderer(SDL_Renderer *renderer);

  /**
   * @brief Checks if texture is loaded
   *
   * @return true if texture has loaded an image file
   * @return false if texture is not loaded
   */
  const bool isLoaded() noexcept;

  /**
   * @brief Frees the texture object from memory
   */
  void destroy();
};
} // namespace gdk

#endif
