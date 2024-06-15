#include <animatedtexture.hpp>

GDK_AnimatedTexture::GDK_AnimatedTexture(){};

GDK_AnimatedTexture::GDK_AnimatedTexture(SDL_Renderer *renderer) {
  setRenderer(renderer);
}

GDK_AnimatedTexture::GDK_AnimatedTexture(SDL_Renderer *renderer,
                                         const char *tile_sheet_path,
                                         unsigned int tile_count) {
  setRenderer(renderer);
  loadTileSheet(tile_sheet_path, tile_count);
}

GDK_AnimatedTexture::GDK_AnimatedTexture(SDL_Renderer *renderer,
                                         const char *tile_sheet_path,
                                         unsigned int tile_count,
                                         unsigned int tile_duration) {
  setRenderer(renderer);
  loadTileSheet(tile_sheet_path, tile_count);
  setTileDuration(tile_duration);
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
  }

  setTile(current_tile);

  last_tile_tick = SDL_GetTicks();
  current_tile++;
}

void GDK_AnimatedTexture::setTile(unsigned int tile_index) {
  sdl_texture = tiles.at(tile_index);
}

void GDK_AnimatedTexture::loadTileSheet(const char *tile_sheet_path,
                                        unsigned int tile_count) {
  if (tile_count < 1) {
    printf(ERR_COLOR "GDK ERROR:" DEF_COLOR
                     " Failed creating tiles - tile count is less than 1\n");
    return;
  }

  if (renderer == nullptr) {
    printf(ERR_COLOR "GDK ERROR:" DEF_COLOR
                     " Failed creating tile sheet with file path" FPATH_COLOR
                     " %s" DEF_COLOR " (renderer not set)\n",
           tile_sheet_path);
    return;
  }

  // Set parameters
  this->tile_count = tile_count;
  this->tile_sheet_path = tile_sheet_path;
  current_tile = 0;

  // Load surface
  surface = IMG_Load(tile_sheet_path);

  if (surface == NULL) {
    printf(ERR_COLOR "GDK ERROR:" DEF_COLOR
                     " Failed loading tile sheet with file path" FPATH_COLOR
                     " %s\n",
           tile_sheet_path);
    return;
  }

  // Destroy old and create new tile sheet texture
  SDL_DestroyTexture(tile_sheet);
  tile_sheet = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_FreeSurface(surface);

  // Set tile box width and height
  int tile_sheet_width, tile_sheet_height;
  SDL_QueryTexture(tile_sheet, NULL, NULL, &tile_sheet_width,
                   &tile_sheet_height);

  tile_box.w = tile_sheet_width / tile_count;
  tile_box.h = tile_sheet_height;

  // Delete previous tiles
  for (int i = 0; i < tiles.size(); i++) {
    SDL_DestroyTexture(tiles.at(i));
  }

  tiles.clear();

  // Cut individual tiles and place them into vector
  tile_box.x = 0;

  for (int i = 0; i < tile_count; i++) {
    SDL_Texture *tmp_tile =
        SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                          SDL_TEXTUREACCESS_TARGET, tile_box.w, tile_box.h);

    tile_box.x = tile_box.w * i;
    SDL_SetRenderTarget(renderer, tmp_tile);
    SDL_SetTextureBlendMode(tmp_tile, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(renderer, tile_sheet, &tile_box, NULL);
    SDL_SetRenderTarget(renderer, NULL);

    tiles.push_back(tmp_tile);
  }

  setTile(0);
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