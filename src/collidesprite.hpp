#ifndef COLLIDE_SPRITE_HPP
#define COLLIDE_SPRITE_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>

#include "basicsprite.hpp"

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
    bool MovementCollided(CollideSprite *collide_sprite) {
        int collider_hbx = collide_sprite->GetHitbox()->x;
        int collider_hby = collide_sprite->GetHitbox()->y;
        int collider_hbw = collide_sprite->GetHitbox()->w;
        int collider_hbh = collide_sprite->GetHitbox()->h;

        if (hitbox.x + hitbox.w > collider_hbx &&
            hitbox.x < collider_hbx + collider_hbw &&
            hitbox.y + hitbox.h > collider_hby &&
            hitbox.y < collider_hby + collider_hbh) {

            this->grad_mvmt_iter = 0;
            return true;
        } else {
            return false;
        }
    }

    // Update hitbox x and y to match offset when moving sprite
    void UpdateHitboxCoords() {
        hitbox.x = x + hitbox_xoffset;
        hitbox.y = y + hitbox_yoffset;
    }

  public:
    CollideSprite(double x, double y, int width, int height,
                  const char *img_path, double speed = 0,
                  bool is_collider = true, bool auto_set_size = false,
                  double rotation_angle = 0,
                  SDL_RendererFlip flip = SDL_FLIP_NONE)
        : BasicSprite(x, y, width, height, img_path, speed, auto_set_size,
                      rotation_angle, flip) {
        SetHitboxParams(0, 0, w, h);
    }

    // Set x position of sprite, factoring in collision
    void SetX(double x, bool check_collision = true) {
        this->x = x;
        UpdateHitboxCoords();

        if (check_collision) {
            for (int i = 0; i < colliders->size(); i++) {
                double collider_x = colliders->at(i)->GetHitbox()->x;
                double collider_w = colliders->at(i)->GetHitbox()->w;

                if (colliders->at(i) != this &&
                    MovementCollided(colliders->at(i))) {

                    if (collider_x > hitbox.x) {
                        this->x = collider_x - hitbox.w - hitbox_xoffset;
                        UpdateHitboxCoords();
                    } else {
                        this->x = collider_x + collider_w - hitbox_xoffset;
                        UpdateHitboxCoords();
                    }

                    return;
                }
            }
        }
    }

    // Set y position of sprite, factoring in collision
    void SetY(double y, bool check_collision = true) {
        this->y = y;
        UpdateHitboxCoords();

        if (check_collision) {
            for (int i = 0; i < colliders->size(); i++) {
                double collider_y = colliders->at(i)->GetHitbox()->y;
                double collider_h = colliders->at(i)->GetHitbox()->h;

                if (colliders->at(i) != this &&
                    MovementCollided(colliders->at(i))) {

                    if (collider_y > hitbox.y) {
                        this->y = collider_y - hitbox.h - hitbox_yoffset;
                        UpdateHitboxCoords();
                    } else {
                        this->y = collider_y + collider_h - hitbox_yoffset;
                        UpdateHitboxCoords();
                    }

                    return;
                }
            }
        }
    }
    // Move sprite along x axis by value of x, checks collisions
    void MoveX(double x) override { SetX(this->x + x, true); }

    // Move sprite along x axis by value of x
    void MoveX(double x, bool check_collision) {
        SetX(this->x + x, check_collision);
    }

    // Move sprite along x axis by value of speed, checks collision
    void MoveX(Direction direction) override {
        MoveX(speed * ConvertDirectionToInt(direction), true);
    }

    // Move sprite along x axis by value of speed
    void MoveX(Direction direction, bool check_collision) {
        MoveX(speed * ConvertDirectionToInt(direction), check_collision);
    }

    // Move sprite along y axis by value of y, checks collisions
    void MoveY(double y) override { SetY(this->y + y, true); }

    // Move sprite along y axis by value of y
    void MoveY(double y, bool check_collision) {
        SetY(this->y + y, check_collision);
    }

    // Move sprite along y axis by value of speed, checks collisions
    void MoveY(Direction direction) override {
        MoveY(speed * ConvertDirectionToInt(direction), true);
    }

    // Move sprite along y axis by value of speed
    void MoveY(Direction direction, bool check_collision) {
        MoveY(speed * ConvertDirectionToInt(direction), check_collision);
    }

    // Set pointer to vector containing all sprites for which collisions should
    // be checked
    // Automatically set when sprite is added to scene
    // is_collider has no effect on this
    void SetColliders(std::vector<CollideSprite *> *colliders) {
        this->colliders = colliders;
    }

    // Sets hitbox x, y, width and height
    // x_offset and y_offset are hitbox coordinates relative to sprite x and y
    void SetHitboxParams(double x_offset, double y_offset, int width,
                         int height) {
        hitbox_xoffset = x_offset;
        hitbox_yoffset = y_offset;
        hitbox.x = x + x_offset;
        hitbox.y = y + y_offset;
        hitbox.w = width;
        hitbox.h = height;
    }

    SDL_Rect *GetHitbox() { return &hitbox; }

    // Returns true if this and collide_sprite are touching/overlapping
    // Does not check if collide_sprite is visible
    bool Collided(CollideSprite *collide_sprite) {
        int collider_hbx = collide_sprite->GetHitbox()->x;
        int collider_hby = collide_sprite->GetHitbox()->y;
        int collider_hbw = collide_sprite->GetHitbox()->w;
        int collider_hbh = collide_sprite->GetHitbox()->h;

        if (hitbox.x + hitbox.w >= collider_hbx &&
            hitbox.x <= collider_hbx + collider_hbw &&
            hitbox.y + hitbox.h >= collider_hby &&
            hitbox.y <= collider_hby + collider_hbh && collide_sprite != this) {

            this->collide_sprite = collide_sprite;
            return true;
        } else {
            return false;
        }
    }

    // Returns true if sprite collided with any other sprite from vector set
    // with SetColliders()
    // Does not check sprite visibility
    bool Collided() {
        for (int i = 0; i < colliders->size(); i++) {

            if (Collided(colliders->at(i))) {
                return true;
            }
        }

        return false;
    }

    // Returns the last sprite this sprite collided with
    // It is a good idea to only call GetCollideSprite() after checking for
    // collision with Collided() to make sure collision sprite is not outdated
    CollideSprite *GetCollideSprite() { return collide_sprite; }
};

#endif