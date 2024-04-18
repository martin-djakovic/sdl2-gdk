#ifndef COLLIDE_SPRITE_HPP
#define COLLIDE_SPRITE_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <sprite.hpp>
#include <vector>

// Image that represents an object in-game
// Has basic functions for movement, drawing and collisions
class GDK_CollideSprite : public GDK_Sprite {
  friend class GDK_Scene;

protected:
  SDL_Rect hitbox;
  double hitbox_xoffset;
  double hitbox_yoffset;
  bool enable_movement_collision = true;
  std::vector<GDK_CollideSprite *> *colliders;
  GDK_CollideSprite *collide_sprite;

  /**
   * @brief Only used to check for collisions when updating sprite x and y.
   * Does not return true on surface collision (when 2 sprites are touching,
   * but not overlapping)
   *
   * @param collide_sprite GDK_CollideSprite to check collisions with
   * @return true if this spirte collided with collide_sprite
   * @return false if this spirte did not collide with collide_sprite
   */
  const bool movementCollided(GDK_CollideSprite *collide_sprite);

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
  GDK_CollideSprite();
  /**
   * @brief Sets texture and default values for variables. Hitbox is not
   * initialized, so it will have to be initialized later with
   * setHitboxProperties()
   *
   * @param texture sprite texture
   */
  GDK_CollideSprite(GDK_ImageTexture *texture);
  /**
   * @brief Sets texture, x, y, width and height of sprite. Initializes hitbox
   *
   * @param texture sprite texture
   * @param x x coordinate of sprite
   * @param y y coordinate of sprite
   * @param width sprite width
   * @param height sprite height
   */
  GDK_CollideSprite(GDK_ImageTexture *texture, double x, double y, int width,
                    int height);

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
  void setColliders(std::vector<GDK_CollideSprite *> *colliders);

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
  const bool collided(GDK_CollideSprite *collide_sprite);

  /**
   * @brief Checks if sprite collided (touched/overlapped) with
   * any sprite from vector set with setColliders()
   *
   * @return true if sprite collided
   * @return false if sprite did not collide
   */
  const bool collided();

  /**
   * @brief Get the last sprite this sprite collided with. Make sure to check
   * if sprite collided before calling this function or you will get
   * inaccurate results
   */
  GDK_CollideSprite *getCollideSprite();
};

#endif
