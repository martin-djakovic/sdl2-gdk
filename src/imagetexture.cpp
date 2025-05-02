#include <errorcolors.hpp>
#include <imagetexture.hpp>

namespace gdk {
ImageTexture::ImageTexture() {}

ImageTexture::ImageTexture(SDL_Renderer *renderer) {
  setRenderer(renderer);
}

ImageTexture::ImageTexture(SDL_Renderer *renderer,
                                   const char *file_path) {
  setRenderer(renderer);
  loadImageFile(file_path);
}

ImageTexture::ImageTexture(ImageTexture *texture){
  setRenderer(texture->renderer);
  loadImageFile(texture->file_path);
}

void ImageTexture::loadImageFile(const char *file_path) {
  surface = IMG_Load(file_path);

  // Check if given image is valid
  if (surface == NULL) {
    printf(ERR_COLOR "GDK ERROR:" DEF_COLOR
                     " Failed loading image with file path" FPATH_COLOR
                     " %s\n" DEF_COLOR,
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
    printf(ERR_COLOR "GDK ERROR:" DEF_COLOR
                     " Failed creating texture with file path" FPATH_COLOR
                     " %s" DEF_COLOR " (renderer not set)\n",
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