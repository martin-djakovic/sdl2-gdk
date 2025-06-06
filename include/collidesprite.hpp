#ifndef COLLIDE_SPRITE_HPP
#define COLLIDE_SPRITE_HPP

#ifdef __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <sdl2-gdk/sprite.hpp>
#include <sdl2-gdk/apiprefix.h>

#elif _WIN32
#include <SDL.h>
#include <SDL_image.h>
#include <sprite.hpp>
#include <apiprefix.h>

#elif __APPLE__
#include <SDL.h>
#include <SDL_image.h>
#include <sprite.hpp>
#include <apiprefix.h>

#else
#error "FATAL ERROR: Operating system not supported"

#endif

#include <vector>

namespace gdk {
class API_PREFIX CollideSprite : public Sprite {
  friend class Scene;
  friend class Camera;

protected:
  SDL_Rect hitbox;
  double hitbox_xoffset;
  double hitbox_yoffset;
  bool enable_movement_collision = true;
  std::vector<CollideSprite *> *colliders = nullptr;
  std::vector<CollideSprite *> collided_sprites;

  /**
   * @brief Only used to check for collisions when updating sprite x and y.
   * Does not return true on surface collision (when 2 sprites are touching,
   * but not overlapping)
   *
   * @param collide_sprite GDK_CollideSprite to check collisions with
   * @return true if this spirte collided with collide_sprite
   * @return false if this spirte did not collide with collide_sprite
   */
  const bool movementCollided(CollideSprite *collide_sprite);

  /**
   * @brief Updates hitbox coordinates when moving sprite to match properties
   * set with setHitboxProperties()
   */
  void updateHitboxCoords() noexcept;

public:
  /**
   * @brief Sets default values for variables. Hitbox is not initialized, so
   * it will have to be initialized later with setHitboxProperties()
   */
  CollideSprite();
  /**
   * @brief Sets texture and default values for variables. Hitbox is not
   * initialized, so it will have to be initialized later with
   * setHitboxProperties()
   *
   * @param texture sprite texture
   */
  CollideSprite(ImageTexture *texture, int zindex = 0);
  /**
   * @brief Sets texture, x, y, width and height of sprite. Initializes hitbox
   */
  CollideSprite(ImageTexture *texture, double x, double y, int width,
                int height, int zindex = 0);
  /**
   * @brief Sets texture and default values for variables. Hitbox is not
   * initialized, so it will have to be initialized later with
   * setHitboxProperties()
   *
   * @param texture sprite texture
   */
  CollideSprite(AnimatedTexture *texture, int zindex = 0);
  /**
   * @brief Sets texture, x, y, width and height of sprite. Initializes hitbox
   */
  CollideSprite(AnimatedTexture *texture, double x, double y, int width,
                int height, int zindex = 0);

  CollideSprite(CollideSprite *sprite);

  CollideSprite(CollideSprite *sprite, double x, double y);

  /**
   * @brief Enable/disable movement collisions. Enabled by default
   *
   * @param enable_movement_collision if true, sprite collides during movement
   * and interferes with other moving collide sprites. If false, sprite
   * doesn't collide during movement or interfere with other moving sprites.
   */
  void setEnableMovementCollision(bool enable_movement_collision) noexcept;

  /**
   * @return true if sprite collides during movement. This is the default
   * value
   * @return false if sprite doesn't collide during movement
   */
  const bool getEnableMovementCollision() noexcept;

  /**
   * @brief Set the position of sprite. Checks collisions
   *
   * @param x x coordinate of sprite
   * @param y y coordinate of sprite
   */
  void setPosition(double x, double y) noexcept override;

  /**
   * @brief Move sprite relative to it's current position. Checks collisions
   *
   * @param x distance to move on x axis
   * @param y distance to move on y axis
   */
  void move(double x, double y) noexcept override;

  /**
   * @brief Defines all sprites with which this sprite can collide during
   * movement, or when calling collided() function with no parameters
   *
   * @param colliders pointer to vector containing all sprites with which this
   * sprite can implicitly collide with
   */
  void setColliders(std::vector<CollideSprite *> *colliders);

  /**
   * @brief Set hitbox properties of collide sprite
   *
   * @param x_offset hitbox offset on the x axis relative to sprite
   * coordinates
   * @param y_offset hitbox offset on the y axis relative to sprite
   * coordinates
   * @param width hitbox width
   * @param height hitbox height
   */
  void setHitboxProperties(double x_offset, double y_offset, int width,
                           int height);

  /**
   * @return pointer to rectangle with sprite hitbox properties
   */
  SDL_Rect *getHitbox();

  /**
   * @brief Checks if sprite collided (touched/overlapped) with
   * @param collide_sprite
   *
   * @param collide_sprite sprite to check collision with
   * @return true if this sprite collided with @param collide_sprite
   * @return false if this sprite did not collide with @param collide_sprite
   */
  const bool collided(CollideSprite *collide_sprite);

  /**
   * @brief Checks if sprite collided (touched/overlapped) with
   * any sprite from vector set with setColliders()
   *
   * @return true if sprite collided
   * @return false if sprite did not collide
   */
  const bool collided();

  /**
   * @brief Get the last sprites this sprite collided with. Sprites are added to
   * the vector only when collided() returns true, or removed when it returns
   * false. To get accurate results you must call collided() first
   */
  std::vector<CollideSprite *> *getCollidedSprites();
};
} // namespace gdk

#endif