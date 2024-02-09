#ifndef COLLIDE_SPRITE_HPP
#define COLLIDE_SPRITE_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <basicsprite.hpp>
#include <string>
#include <vector>

// Image that represents an object in-game
// Has basic functions for movement, drawing and collisions
class CollideSprite : public BasicSprite {

  protected:
    SDL_Rect hitbox;
    double hitbox_xoffset;
    double hitbox_yoffset;
    std::vector<CollideSprite *> *colliders;
    CollideSprite *collide_sprite;

    // MovementCollided() is only used to check for collisions when updating
    // sprite x and y. Does not return true on surface collision (when 2 sprites
    // are touching, but not overlapping)
    bool MovementCollided(CollideSprite *collide_sprite);

    // Update hitbox x and y to match offset when moving sprite
    void UpdateHitboxCoords();

  public:
    CollideSprite(double x, double y, int width, int height,
                  const char *img_path, double speed = 0,
                  bool auto_set_size = false, double rotation_angle = 0,
                  SDL_RendererFlip flip = SDL_FLIP_NONE);

    // Set x position of sprite, factoring in collision
    void SetX(double x, bool check_collision = true);

    // Set y position of sprite, factoring in collision
    void SetY(double y, bool check_collision = true);

    // Move sprite along x axis by value of x, checks collisions
    void MoveX(double x) override;

    // Move sprite along x axis by value of x
    void MoveX(double x, bool check_collision);

    // Move sprite along x axis by value of speed, checks collision
    void MoveX(GDK_Direction direction) override;

    // Move sprite along x axis by value of speed
    void MoveX(GDK_Direction direction, bool check_collision);

    // Move sprite along y axis by value of y, checks collisions
    void MoveY(double y) override;

    // Move sprite along y axis by value of y
    void MoveY(double y, bool check_collision);

    // Move sprite along y axis by value of speed, checks collisions
    void MoveY(GDK_Direction direction) override;

    // Move sprite along y axis by value of speed
    void MoveY(GDK_Direction direction, bool check_collision);

    // Set pointer to vector containing all sprites for which collisions should
    // be checked
    // Automatically set when sprite is added to scene
    // is_collider has no effect on this
    void SetColliders(std::vector<CollideSprite *> *colliders);

    // Sets hitbox x, y, width and height
    // x_offset and y_offset are hitbox coordinates relative to sprite x and y
    void SetHitboxParams(double x_offset, double y_offset, int width,
                         int height);

    SDL_Rect *GetHitbox();

    // Returns true if this and collide_sprite are touching/overlapping
    // Does not check if collide_sprite is visible
    bool Collided(CollideSprite *collide_sprite);

    // Returns true if sprite collided with any other sprite from vector set
    // with SetColliders()
    // Does not check sprite visibility
    bool Collided();

    // Returns the last sprite this sprite collided with
    // It is a good idea to only call GetCollideSprite() after checking for
    // collision with Collided() to make sure collision sprite is not outdated
    CollideSprite *GetCollideSprite();
};

#endif