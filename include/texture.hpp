#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class GDK_Texture {
  friend class GDK_Sprite;

private:
  const char *file_path;
  SDL_Texture *sdl_texture = nullptr;
  SDL_Surface *surface;
  SDL_Renderer *renderer = nullptr;

public:
  GDK_Texture();
  /**
   * @param renderer renderer on which texture will be created and drawn on
   */
  GDK_Texture(SDL_Renderer *renderer);

  /**
   * @param renderer renderer on which texture will be created and drawn on
   * @param file_path relative/absolute file path to the image that will be used
   * to create texture. All file types supported by
   * SDL_Texture are compatible
   */
  GDK_Texture(SDL_Renderer *renderer, const char *file_path);

  /**
   * @brief Generates a texture from image file. All file types supported by
   * SDL_Texture are compatible with this function. Renderer must be set with
   * setRenderer() or in constructor before calling loadImageFile()
   *
   * @param file_path relative/absolute file path to the image
   */
  void loadImageFile(const char *file_path);

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

  /*
   * @brief Frees the texture object from memory
   */
  void destroy();
};

#endif
