#ifndef ANIMATED_TEXTURE_HPP
#define ANIMATED_TEXTURE_HPP

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

#include <sdl2-gdk/errorcolors.hpp>
#include <sdl2-gdk/texture.hpp>
#include <vector>

namespace gdk {
class AnimatedTexture : public Texture {
private:
  SDL_Rect tile_box = {0, 0, 0, 0};
  int tile_count = 0;
  int current_tile = 0;
  int tile_duration = 0;
  bool is_playing = true;
  const char *tile_sheet_path = "";
  SDL_Surface *surface = nullptr;
  SDL_Texture *tile_sheet = nullptr;
  std::vector<SDL_Texture *> tiles;
  int last_tile_tick = 0;

protected:
  /**
   * @brief switches to next tile on draw event if tile duration has passed
   */
  void flagDrawEvent() override;

  void setTile(unsigned int tile_index);

public:
  AnimatedTexture();

  AnimatedTexture(SDL_Renderer *renderer);

  AnimatedTexture(SDL_Renderer *renderer, const char *tile_sheet_path,
                  unsigned int tile_count);

  /**
   * @param tile_duration Time between switching tiles in milliseconds
   */
  AnimatedTexture(SDL_Renderer *renderer, const char *tile_sheet_path,
                  unsigned int tile_count, unsigned int tile_duration);

  AnimatedTexture(AnimatedTexture *texture);

  /**
   * @brief Loads the image from which tiles will be made. All tiles are equal
   * width and heigth. The tile height is equal to the height of the tile sheet
   *
   * @param tile_sheet_path relative/absolute file path to the tile sheet. All
   * file types supported by SDL_Texture are compatible
   * @param tile_count number of tiles in tile sheet (must be greater than 0)
   */
  void loadTileSheet(const char *tile_sheet_path, unsigned int tile_count);

  const unsigned int getTileCount() noexcept;

  /**
   * @brief Set time between switching tiles
   * @param time_ms Tile duration in milliseconds
   */
  void setTileDuration(unsigned int time_ms) noexcept;
  
  /**
   * @return Tile duration in milliseconds
   */
  const unsigned int getTileDuration() noexcept;

  void play() noexcept;

  void pause() noexcept;

  const bool isPlaying() noexcept;

  /**
   * @brief Sets animation to first tile
   */
  void reset() noexcept;

  const bool isLoaded() noexcept;
};
} // namespace gdk

#endif