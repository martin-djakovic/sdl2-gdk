#include <sdl2-gdk/errorcolors.hpp>
#include <sdl2-gdk/imagetexture.hpp>

namespace gdk {
ImageTexture::ImageTexture() {}

ImageTexture::ImageTexture(SDL_Renderer *renderer) { setRenderer(renderer); }

ImageTexture::ImageTexture(SDL_Renderer *renderer, const char *file_path) {
  setRenderer(renderer);
  loadImageFile(file_path);
}

ImageTexture::ImageTexture(ImageTexture *texture) {
  setRenderer(texture->renderer);
  loadImageFile(texture->file_path);
}

void ImageTexture::loadImageFile(const char *file_path) {
  surface = IMG_Load(file_path);

  // Check if given image is valid
  if (surface == NULL) {
    printf(GDK_ERROR_COLOR
           "GDK ERROR:" GDK_DEFAULT_COLOR
           " Failed loading image with file path" GDK_FILE_PATH_COLOR
           " %s\n" GDK_DEFAULT_COLOR,
           file_path);
    return;
  }

  // Destroy previous texture
  SDL_DestroyTexture(sdl_texture);

  // Texture cannot be created if renderer is not set
  if (renderer != nullptr) {
    sdl_texture = SDL_CreateTextureFromSurface(renderer, surface);
    this->file_path = file_path;
  } else {
    printf(GDK_ERROR_COLOR
           "GDK ERROR:" GDK_DEFAULT_COLOR
           " Failed creating texture with file path" GDK_FILE_PATH_COLOR
           " %s" GDK_DEFAULT_COLOR " (renderer not set)\n",
           file_path);
  }

  SDL_FreeSurface(surface);
}

const bool ImageTexture::isLoaded() noexcept {
  if (sdl_texture == nullptr) {
    return false;
  } else {
    return true;
  }
}
} // namespace gdk