#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>

// Image that represents an object in-game
class Sprite {

  public:
    // Width
    int w;
    // Height
    int h;
    // If true, sprite cannot move past other colliders when setting x and y
    bool is_collider;

    Sprite(int x, int y, int width, int height, const char *img_path,
           bool is_collider = true, bool auto_set_size = false,
           double rotation_angle = 0, SDL_RendererFlip flip = SDL_FLIP_NONE) {
        this->x = x;
        this->y = y;
        this->w = width;
        this->h = height;
        this->img_path = img_path;
        this->auto_set_size = auto_set_size;
        this->is_collider = is_collider;
        this->rotation_angle = rotation_angle;
        this->flip = flip;
    }

    void Destroy() { SDL_DestroyTexture(texture); }

    int GetX() { return x; }

    // Set x position of sprite
    void SetX(int x, bool check_collision = true) {
        this->x = x;

        if (this->is_collider && check_collision) {
            for (int i = 0; i < colliders->size(); i++) {
                int collider_x = colliders->at(i)->GetX();
                int collider_w = colliders->at(i)->w;

                if (colliders->at(i) != this && colliders->at(i)->is_collider &&
                    MovementCollided(colliders->at(i))) {

                    if (collider_x > x) {
                        this->x = collider_x - w;
                    } else {
                        this->x = collider_x + collider_w;
                    }

                    return;
                }
            }
        }
    }

    // Move sprite along x axis
    void MoveX(int x, bool check_collision = true) {
        SetX(this->x + x, check_collision);
    }

    int GetY() { return y; }

    // Set y position of sprite
    void SetY(int y, bool check_collision = true) {
        this->y = y;

        if (this->is_collider && check_collision) {
            for (int i = 0; i < colliders->size(); i++) {
                int collider_y = colliders->at(i)->GetY();
                int collider_h = colliders->at(i)->h;

                if (colliders->at(i) != this && colliders->at(i)->is_collider &&
                    MovementCollided(colliders->at(i))) {

                    if (collider_y > y) {
                        this->y = collider_y - h;
                    } else {
                        this->y = collider_y + collider_h;
                    }

                    return;
                }
            }
        }
    }

    // Move sprite along y axis
    void MoveY(int y, bool check_collision = true) {
        SetY(this->y + y, check_collision);
    }

    // Set sprite renderer and create image passed in constructor
    // Renderer will be automatically set when sprite is added to a scene
    void SetRenderer(SDL_Renderer *renderer) {
        sprite_renderer = renderer;

        hitbox.w = w;
        hitbox.h = h;

        surface = IMG_Load(img_path.c_str());
        texture = SDL_CreateTextureFromSurface(sprite_renderer, surface);
        SDL_FreeSurface(surface);

        if (auto_set_size) {
            SDL_QueryTexture(texture, NULL, NULL, &w, &h);
        }
    }

    // Set pointer to vector containing all sprites for which collisions should
    // be checked
    // Automatically set when sprite is added to scene
    // is_collider has no effect on this
    void SetColliders(std::vector<Sprite *> *colliders) {
        this->colliders = colliders;
    }

    // Change sprite image after an image has already been set with SetImg()
    void ChangeImg(const char *img_path, bool param_auto_set_size = false) {
        hitbox.w = w;
        hitbox.h = h;

        surface = IMG_Load(img_path);
        texture = SDL_CreateTextureFromSurface(sprite_renderer, surface);
        SDL_FreeSurface(surface);

        if (param_auto_set_size) {
            SDL_QueryTexture(texture, NULL, NULL, &w, &h);
        }
    }

    // Set sprite flip to SDL_FLIP_NONE, SDL_FLIP_HORIZONTAL or SDL_FLIP_VERTICAL
    void SetFlip(SDL_RendererFlip flip) { this->flip = flip; }

    // Sets sprite rotation to angle
    void SetRotation(double angle) { rotation_angle = angle; }

    // Changes sprite rotation by angle
    void Rotate(double angle) { SetRotation(rotation_angle + angle); }

    void Draw() {
        hitbox.x = x;
        hitbox.y = y;

        SDL_RenderCopyEx(sprite_renderer, texture, NULL, &hitbox,
                         rotation_angle, NULL, flip);
    }

    // Checks if sprite is in window bounds
    bool IsVisible(int win_w, int win_h) {
        if (x + w > 0 && y + w > 0 && x < win_w && y < win_h) {
            return true;
        } else {
            return false;
        }
    }

    // Returns true if this sprite and collide_sprite collided
    // Does not check if collide_sprite is visible
    bool Collided(Sprite *collide_sprite, bool check_is_collider = true) {
        int sprite_x = collide_sprite->GetX();
        int sprite_y = collide_sprite->GetY();
        int sprite_w = collide_sprite->w;
        int sprite_h = collide_sprite->h;

        if (bool check_is_collider = true) {
            if (x + w >= sprite_x && x <= sprite_x + sprite_w &&
                y + h >= sprite_y && y <= sprite_y + sprite_h &&
                collide_sprite->is_collider) {

                this->collide_sprite = collide_sprite;
                return true;
            } else {
                return false;
            }
        } else {
            if (x + w >= sprite_x && x <= sprite_x + sprite_w &&
                y + h >= sprite_y && y <= sprite_y + sprite_h) {

                this->collide_sprite = collide_sprite;
                return true;
            } else {
                return false;
            }
        }
    }

    // Returns true if sprite collided with any other sprite from vector set
    // with SetColliders()
    // Does not check if colliders are visible
    // Will return false on collision if check_is_collider = true and sprite
    // is_collider = false
    // Does not check sprite visibility
    bool Collided(bool check_is_collider = true) {
        for (int i = 0; i < colliders->size(); i++) {

            if (colliders->at(i) != this &&
                colliders->at(i)->is_collider == true &&
                Collided(colliders->at(i), check_is_collider)) {

                return true;
            }
        }

        return false;
    }

    // Returns the last sprite this sprite collided with
    // It is a good idea to only call GetCollideSprite() after checking for
    // collision with Collided() to make sure collision sprite is not outdated
    Sprite *GetCollideSprite() { return collide_sprite; }

  private:
    int x;
    int y;
    double rotation_angle;
    std::string img_path;
    SDL_RendererFlip flip;
    SDL_Renderer *sprite_renderer;
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect hitbox;
    std::vector<Sprite *> *colliders;
    Sprite *collide_sprite;
    bool auto_set_size;

    // MovementCollided() is only used to check for collisions when updating
    // sprite x and y. Does not return true on surface collision (when 2 sprites
    // are touching, but not overlapping)
    bool MovementCollided(Sprite *collide_sprite) {
        int sprite_x = collide_sprite->GetX();
        int sprite_y = collide_sprite->GetY();
        int sprite_w = collide_sprite->w;
        int sprite_h = collide_sprite->h;

        if (x + w > sprite_x && x < sprite_x + sprite_w && y + h > sprite_y &&
            y < sprite_y + sprite_h && collide_sprite->is_collider) {

            return true;
        } else {
            return false;
        }
    }
};

#endif