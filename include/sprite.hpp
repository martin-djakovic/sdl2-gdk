#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "fonttexture.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <errorcolors.hpp>
#include <imagetexture.hpp>

class GDK_Sprite {
  friend class GDK_Scene;

protected:
  double x;
  double y;
  int width;
  int height;
  SDL_Rect rect;
  GDK_Texture *texture = nullptr;
  SDL_RendererFlip flip = SDL_FLIP_NONE;
  SDL_Point *rotation_point = nullptr;
  double rotation_angle;

  // Variables used for gradual movement across multiple frames
  int grad_mvmt_iter = 0;
  double grad_mvmt_speedx;
  double grad_mvmt_speedy;
  double grad_mvmt_goalx;
  double grad_mvmt_goaly;

  /**
   * @brief Moves sprite each frame according to data calculated in moveTo()
   */
  virtual void gradualMovementIterator();

public:
  GDK_Sprite();
  GDK_Sprite(GDK_ImageTexture *texture);
  GDK_Sprite(GDK_ImageTexture *texture, double x, double y, int width, int height);
  GDK_Sprite(GDK_FontTexture *texture);
  GDK_Sprite(GDK_FontTexture *texture, double x, double y);
  GDK_Sprite(GDK_FontTexture *texture, double x, double y, int width, int height);

  /*
   * @brief Set the position of sprite
   *
   * @param x x coordinate of sprite
   * @param y y coordinate of sprite
   */
  virtual void setPosition(double x, double y) noexcept;

  /*
   * @return x position of sprite
   */
  const double getX() noexcept;
  /*
   * @return y position of sprite
   */
  const double getY() noexcept;

  /*
   * @brief Move sprite
   *
   * @param x distance to move on x-axis
   * @param y distance to move on y-axis
   */
  virtual void move(double x, double y) noexcept;

  /*
   * @brief Set width of sprite
   */
  void setWidth(unsigned int width) noexcept;
  /*
   * @return Width of sprite
   */
  const unsigned int getWidth() noexcept;

  /*
   * @brief Set height of sprite
   */
  void setHeight(unsigned int height) noexcept;
  /*
   * @return Height of sprite
   */
  const unsigned int getHeight() noexcept;

  /*
   * @brief Moves sprite to given position by given speed evenly across
   * multiple frames
   *
   * @param x x coordinate destination
   * @param y y coordinate destination
   * @param speed movement speed
   */
  void moveTo(int x, int y, double speed);

  /*
   * @brief Set the sprite texture
   */
  void setTexture(GDK_ImageTexture *texture);

  /*
   * @brief Set the sprite texture to a font texture
   *
   * @param auto_set_size determine if sprite width/height should be
   * automatically adjusted to fit the text without stretching/compressing it*/
  void setTexture(GDK_FontTexture *texture, const bool auto_set_size = true);

  /**
   * @brief Flip the sprite texture
   *
   * @param flip SDL_FLIP_NONE, SDL_FLIP_HORIZONTAL or
   * SDL_FLIP_VERTICAL
   */
  void setFlip(SDL_RendererFlip flip);

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
};

#endif
