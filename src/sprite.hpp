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
           bool is_collider = true, bool auto_set_size = false) {
        this->x = x;
        this->y = y;
        this->w = width;
        this->h = height;
        this->img_path = img_path;
        this->auto_set_size = auto_set_size;
        this->is_collider = is_collider;
    }

    //~Sprite() {
    //    SDL_DestroyTexture(texture);
    //    delete this;
    //}

    int GetX() { return x; }
    // Should only be used when collision detection is not intended
    void SetX(int x) { this->x = x; }

    // Set x position if sprite does not collide at that position
    void SetX(int x, std::vector<Sprite *> *colliders) {
        this->x = x;

        if (this->is_collider) {
            for (int i = 0; i < colliders->size(); i++) {
                int collider_x = colliders->at(i)->GetX();
                int collider_w = colliders->at(i)->w;

                if (colliders->at(i) != this && colliders->at(i)->is_collider &&
                    Collided(colliders->at(i))) {

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

    int GetY() { return y; }
    // Should only be used when collision detection is not intended
    void SetY(int y) { this->y = y; }

    // Set y position if sprite does not collide at that position
    void SetY(int y, std::vector<Sprite *> *colliders) {
        this->y = y;

        if (this->is_collider) {
            for (int i = 0; i < colliders->size(); i++) {
                int collider_y = colliders->at(i)->GetY();
                int collider_h = colliders->at(i)->h;

                if (colliders->at(i) != this && colliders->at(i)->is_collider &&
                    Collided(colliders->at(i))) {

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

    // Set spite renderer and create image passed in constructor
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

    void Draw() {
        hitbox.x = x;
        hitbox.y = y;

        SDL_RenderCopy(sprite_renderer, texture, NULL, &hitbox);
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
    bool Collided(Sprite *collide_sprite) {
        int sprite_x = collide_sprite->GetX();
        int sprite_y = collide_sprite->GetY();
        int sprite_w = collide_sprite->w;
        int sprite_h = collide_sprite->h;

        if (x + w > sprite_x && x < sprite_x + sprite_w && y + h > sprite_y &&
            y < sprite_y + sprite_h) {
            return true;
        } else {
            return false;
        }
    }

    // Returns true if this sprite and any sprite from vector colliders collided
    // Does not check if colliders are visible
    bool Collided(std::vector<Sprite *> *colliders) {
        for (int i = 0; i < colliders->size(); i++) {

            if (colliders->at(i) != this && Collided(colliders->at(i))) {
                return true;
            }
        }

        return false;
    }

  private:
    int x;
    int y;
    std::string img_path;
    SDL_Renderer *sprite_renderer;
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect hitbox;
    bool auto_set_size;
};

#endif