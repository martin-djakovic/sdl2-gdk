#include <errorcolors.hpp>
#include <texture.hpp>

GDK_Texture::GDK_Texture() {}

GDK_Texture::GDK_Texture(SDL_Renderer *renderer) { setRenderer(renderer); }

GDK_Texture::GDK_Texture(SDL_Renderer *renderer, const char *file_path) {
  setRenderer(renderer);
  loadImageFile(file_path);
}

void GDK_Texture::loadImageFile(const char *file_path) {
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

void GDK_Texture::destroy() {
  SDL_DestroyTexture(sdl_texture);
  printf(NOTICE_COLOR "GDK NOTICE:" DEF_COLOR
                      " GDK_Texture with file path" FPATH_COLOR " %s" DEF_COLOR
                      " destroyed\n",
         file_path);
}
