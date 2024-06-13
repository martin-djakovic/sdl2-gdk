#include <animatedtexture.hpp>

GDK_AnimatedTexture::GDK_AnimatedTexture(){};

GDK_AnimatedTexture::GDK_AnimatedTexture(SDL_Renderer *renderer) {
  setRenderer(renderer);
}

GDK_AnimatedTexture::GDK_AnimatedTexture(SDL_Renderer *renderer,
                                         const char *tile_sheet_path) {
  setRenderer(renderer);
  loadTileSheet(tile_sheet_path);
}

GDK_AnimatedTexture::GDK_AnimatedTexture(SDL_Renderer *renderer,
                                         const char *tile_sheet_path,
                                         unsigned int tile_count) {
  setRenderer(renderer);
  loadTileSheet(tile_sheet_path);
  setTileCount(tile_count);
  loadTile(0);
}

GDK_AnimatedTexture::GDK_AnimatedTexture(SDL_Renderer *renderer,
                                         const char *tile_sheet_path,
                                         unsigned int tile_count,
                                         unsigned int tile_duration) {
  setRenderer(renderer);
  loadTileSheet(tile_sheet_path);
  setTileCount(tile_count);
  setTileDuration(tile_duration);
  loadTile(0);
}

void GDK_AnimatedTexture::flagDrawEvent() {
  if (tile_duration == 0) {
    printf(WARN_COLOR "GDK WARNING:" DEF_COLOR
                      " Tile duration of animated texture is 0\n");
  }

  if (SDL_GetTicks() - last_tile_tick < tile_duration || !isPlaying()) {
    return;
  }

  if (current_tile == tile_count) {
    current_tile = 0;
    tile.x = 0;
  }

  loadTile(current_tile);

  last_tile_tick = SDL_GetTicks();
  current_tile++;
}

void GDK_AnimatedTexture::updateTileSize() {
  if (tile_count > 0) {
    int tile_sheet_width, tile_sheet_height;
    SDL_QueryTexture(tile_sheet, NULL, NULL, &tile_sheet_width,
                     &tile_sheet_height);

    tile.w = tile_sheet_width / tile_count;
    tile.h = tile_sheet_height;

    current_tile = 0;
  }
}

void GDK_AnimatedTexture::loadTile(unsigned int tile_index) {
  tile.x = tile.w * current_tile;

  SDL_DestroyTexture(sdl_texture);
  sdl_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                  SDL_TEXTUREACCESS_TARGET, tile.w, tile.h);

  SDL_SetRenderTarget(renderer, sdl_texture);
  SDL_SetTextureBlendMode(sdl_texture, SDL_BLENDMODE_BLEND);
  SDL_RenderCopy(renderer, tile_sheet, &tile, NULL);
  SDL_SetRenderTarget(renderer, NULL);
}

void GDK_AnimatedTexture::loadTileSheet(const char *tile_sheet_path) {
  this->tile_sheet_path = tile_sheet_path;
  surface = IMG_Load(tile_sheet_path);

  if (surface == NULL) {
    printf(ERR_COLOR "GDK ERROR:" DEF_COLOR
                     " Failed loading tile sheet with file path" FPATH_COLOR
                     " %s\n",
           tile_sheet_path);
    return;
  }

  SDL_DestroyTexture(tile_sheet);

  if (renderer != nullptr) {
    tile_sheet = SDL_CreateTextureFromSurface(renderer, surface);
  } else {
    printf(ERR_COLOR "GDK ERROR:" DEF_COLOR
                     " Failed creating tile sheet with file path" FPATH_COLOR
                     " %s" DEF_COLOR " (renderer not set)\n",
           tile_sheet_path);
  }

  SDL_FreeSurface(surface);
  updateTileSize();
}

void GDK_AnimatedTexture::setTileCount(unsigned int tile_count) noexcept {
  this->tile_count = tile_count;

  if (tile_count < 1) {
    printf(ERR_COLOR "GDK ERROR:" DEF_COLOR
                     " Failed creating tiles - tile count is less than 1\n");
    return;
  }

  updateTileSize();
}

const unsigned int GDK_AnimatedTexture::getTileCount() noexcept {
  return tile_count;
}

void GDK_AnimatedTexture::setTileDuration(unsigned int time_ms) noexcept {
  tile_duration = time_ms;
}

const unsigned int GDK_AnimatedTexture::getTileDuration() noexcept {
  return tile_duration;
}

void GDK_AnimatedTexture::play() noexcept { is_playing = true; }

void GDK_AnimatedTexture::pause() noexcept { is_playing = false; }

const bool GDK_AnimatedTexture::isPlaying() noexcept { return is_playing; }

void GDK_AnimatedTexture::reset() noexcept { current_tile = 0; }

const bool GDK_AnimatedTexture::isLoaded() noexcept {
  return sdl_texture != nullptr && renderer != nullptr;
}