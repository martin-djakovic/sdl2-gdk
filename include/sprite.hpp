#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "fonttexture.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <animatedtexture.hpp>
#include <errorcolors.hpp>
#include <imagetexture.hpp>

namespace gdk {
class Sprite {
  friend class Scene;

protected:
  double x;
  double y;
  int zindex;
  int width;
  int height;
  SDL_Rect rect;
  Texture *texture = nullptr;
  SDL_RendererFlip flip = SDL_FLIP_NONE;
  SDL_Point *rotation_point = nullptr;
  double rotation_angle = 0;

  static const bool comparePtr(Sprite *s1, Sprite *s2);

public:
  Sprite();
  Sprite(ImageTexture *texture, int zindex = 0);
  Sprite(ImageTexture *texture, double x, double y, int width, int height,
         int zindex = 0);
  Sprite(AnimatedTexture *texture, int zindex = 0);
  Sprite(AnimatedTexture *texture, double x, double y, int width, int height,
         int zindex = 0);
  Sprite(FontTexture *texture, int zindex = 0);
  Sprite(FontTexture *texture, double x, double y, int zindex = 0);
  Sprite(FontTexture *texture, double x, double y, int width, int height,
         int zindex = 0);

  /**
   * @brief Set the position of sprite
   *
   * @param x x coordinate of sprite
   * @param y y coordinate of sprite
   */
  virtual void setPosition(double x, double y) noexcept;

  /**
   * @return x position of sprite
   */
  const double getX() noexcept;
  /**
   * @return y position of sprite
   */
  const double getY() noexcept;

  /**
   * @brief Move sprite
   *
   * @param x distance to move on x-axis
   * @param y distance to move on y-axis
   */
  virtual void move(double x, double y) noexcept;

  /**
   * @brief Set width of sprite
   */
  void setWidth(unsigned int width) noexcept;
  /**
   * @return Width of sprite
   */
  const unsigned int getWidth() noexcept;

  /**
   * @brief Set height of sprite
   */
  void setHeight(unsigned int height) noexcept;
  /**
   * @return Height of sprite
   */
  const unsigned int getHeight() noexcept;

  void setTexture(AnimatedTexture *texture);

  /**
   * @brief Set the sprite texture
   */
  void setTexture(ImageTexture *texture);

  /**
   * @brief Set the sprite texture to a font texture
   *
   * @param auto_set_size determine if sprite width/height should be
   * automatically adjusted to fit the text without stretching/compressing it*/
  void setTexture(FontTexture *texture, const bool auto_set_size = true);

  /**
   * @brief Flip the sprite texture
   *
   * @param flip SDL_FLIP_NONE, SDL_FLIP_HORIZONTAL or
   * SDL_FLIP_VERTICAL
   */
  void setFlip(SDL_RendererFlip flip);

  /**
   * @brief Set zindex (draw order) for sprite. Sprite with lower zindex gets
   * drawn first. If two or more sprites have the same zindex, that means the
   * drawing order between those sprites doesn't matter. Make sure to call
   * Scene::updateDrawOrder() whenever changing z-index after sprite has already
   * been added to scene
   */
  void setZindex(int zindex);

  /**
   * @brief Rotate sprite texture to angle
   */
  void setRotation(double angle) noexcept;

  /**
   * @brief Rotate sprite texture by angle
   */
  void rotate(double angle) noexcept;

  /**
   * @brief Set point around which sprite texture rotates, relative to the
   * sprite x and y. Default rotation point is in the center of the sprite
   */
  void setRotationCenter(SDL_Point *rotation_point);

  /**
   * @brief Copies sprite texture to renderer
   */
  void draw();

  /**
   * @brief Check if sprite is in window bounds. The evaluated window is the
   * same window that is associated with the renderer of the texture
   *
   * @return true if sprite is located within the window
   * @return false if sprite is outside the window
   */
  const bool isInBounds();

  /**
   * @brief The Sprite::operator< compares the zindex
   */
  const bool operator<(const Sprite &other_sprite);

  /**
   * @brief The Sprite::operator> compares the zindex
   */
  const bool operator>(const Sprite &other_sprite);
};
} // namespace gdk

#endif
