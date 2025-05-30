#ifndef IMAGE_TEXTURE_HPP
#define IMAGE_TEXTURE_HPP

#ifdef __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#elif _WIN32
#include <SDL.h>
#include <SDL_image.h>

#elif __APPLE__
#include <SDL.h>
#include <SDL_image.h>

#else
#error "FATAL ERROR: Operating system not supported"

#endif

#include <sdl2-gdk/texture.hpp>
#include <sdl2-gdk/apiprefix.h>

namespace gdk {
class API_PREFIX ImageTexture : public Texture {
private:
  const char *file_path;
  SDL_Surface *surface;

public:
  ImageTexture();

  /**
   * @param renderer renderer on which texture will be created and drawn on
   */
  ImageTexture(SDL_Renderer *renderer);

  /**
   * @param renderer renderer on which texture will be created and drawn on
   * @param file_path relative/absolute file path to the image that will be used
   * to create texture. All file types supported by
   * SDL_Texture are compatible
   */
  ImageTexture(SDL_Renderer *renderer, const char *file_path);

  ImageTexture(ImageTexture *texture);

  /**
   * @brief Generates a texture from image file. All file types supported by
   * SDL_Texture are compatible with this function. Renderer must be set with
   * setRenderer() or in constructor before calling loadImageFile()
   *
   * @param file_path relative/absolute file path to the image
   */
  void loadImageFile(const char *file_path);

  /**
   * @brief Checks if texture is loaded
   *
   * @return true if texture has loaded an image file
   * @return false if texture is not loaded
   */
  const bool isLoaded() noexcept;
};
} // namespace gdk

#endif
