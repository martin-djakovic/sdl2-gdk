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
    SDL_Point rotation_center;

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
        rotation_center = {w / 2, h / 2};
    }

    void Destroy() { SDL_DestroyTexture(texture); }

    int GetX() { return round(x); }

    // Set x position of sprite
    void SetX(double x, bool check_collision = true) {
        this->x = x;

        if (this->is_collider && check_collision) {
            for (int i = 0; i < colliders->size(); i++) {
                double collider_x = colliders->at(i)->GetX();
                double collider_w = colliders->at(i)->w;

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
    void MoveX(double x, bool check_collision = true) {
        SetX(this->x + x, check_collision);
    }

    int GetY() { return round(y); }

    // Set y position of sprite
    void SetY(double y, bool check_collision = true) {
        this->y = y;

        if (this->is_collider && check_collision) {
            for (int i = 0; i < colliders->size(); i++) {
                double collider_y = colliders->at(i)->GetY();
                double collider_h = colliders->at(i)->h;

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
    void MoveY(double y, bool check_collision = true) {
        SetY(this->y + y, check_collision);
    }

    // Moves sprite to given point
    // Evenly changes sprite x and y by given speed
    // until point is reached
    void MoveTo(int x, int y, int speed) {
        grad_mvmt_goalx = x;
        grad_mvmt_goaly = y;

        int xdif = this->x - x;
        int ydif = this->y - y;
        // Movement iterations along line between this position and given
        // position
        grad_mvmt_iter = sqrt(xdif * xdif + ydif * ydif) / speed;

        if (grad_mvmt_iter == 0) {
            return;
        }

        grad_mvmt_speedx = -((double)xdif / grad_mvmt_iter);
        grad_mvmt_speedy = -((double)ydif / grad_mvmt_iter);
    }

    // Set sprite renderer and create image passed in constructor
    // Renderer will be automatically set when sprite is added to a scene
    void SetRenderer(SDL_Renderer *renderer) {
        sprite_renderer = renderer;

        img_rect.w = w;
        img_rect.h = h;

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
        img_rect.w = w;
        img_rect.h = h;

        surface = IMG_Load(img_path);
        texture = SDL_CreateTextureFromSurface(sprite_renderer, surface);
        SDL_FreeSurface(surface);

        if (param_auto_set_size) {
            SDL_QueryTexture(texture, NULL, NULL, &w, &h);
        }
    }

    // Set sprite flip to SDL_FLIP_NONE, SDL_FLIP_HORIZONTAL or
    // SDL_FLIP_VERTICAL
    void SetFlip(SDL_RendererFlip flip) { this->flip = flip; }

    // Sets sprite rotation to angle
    void SetRotation(double angle) { rotation_angle = angle; }

    // Changes sprite rotation by angle
    void Rotate(double angle) { SetRotation(rotation_angle + angle); }

    void Draw() {
        img_rect.x = x;
        img_rect.y = y;
        img_rect.w = w;
        img_rect.h = h;

        GradualMovementIterator();

        SDL_RenderCopyEx(sprite_renderer, texture, NULL, &img_rect,
                         rotation_angle, &rotation_center, flip);
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
        // Sprite x and y are doubles,
        // but collisions need to be calculated with ints because sprite is
        // drawn at a whole number coordinate
        int x = GetX();
        int y = GetY();

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
    double x;
    double y;
    double rotation_angle;
    std::string img_path;
    SDL_RendererFlip flip;
    SDL_Renderer *sprite_renderer;
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect img_rect;
    std::vector<Sprite *> *colliders;
    Sprite *collide_sprite;
    bool auto_set_size;

    // Variables used for gradual movement across multiple frames
    int grad_mvmt_iter = 0;
    double grad_mvmt_speedx;
    double grad_mvmt_speedy;
    double grad_mvmt_goalx;
    double grad_mvmt_goaly;

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

    // Used to gradually move sprite according to data from MoveTo()
    // Needed when movement is to be made across multiple frames
    // Is called on every draw
    void GradualMovementIterator() {
        if (grad_mvmt_iter > 0) {
            this->MoveX(grad_mvmt_speedx);
            this->MoveY(grad_mvmt_speedy);

            if (Collided()) {
                grad_mvmt_iter = 0;
            }

            grad_mvmt_iter--;

            if (grad_mvmt_iter == 0 && x != grad_mvmt_goalx &&
                y != grad_mvmt_goalx) {
                this->SetX(grad_mvmt_goalx);
                this->SetY(grad_mvmt_goaly);
            }
        }
    }
};

#endif