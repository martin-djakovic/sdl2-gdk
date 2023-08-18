#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

// Image that represents an object in-game
class Sprite {

  public:
    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;

    Sprite(int x_init, int y_init, int width, int height,
           const char *param_img_path, bool param_auto_set_size = false) {
        x = x_init;
        y = y_init;
        w = width;
        h = height;
        img_path = param_img_path;
        auto_set_size = param_auto_set_size;
    }

    //~Sprite() {
    //    SDL_DestroyTexture(texture);
    //    delete this;
    //}

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

    // Checks if sprite is visible on screen at it's current coordinates
    bool IsVisible(int win_w, int win_h) {
        if (x + w > 0 && y + w > 0 && (x < win_w || y < win_h)) {
            return true;
        } else {
            return false;
        }
    }

  private:
    std::string img_path;
    SDL_Renderer *sprite_renderer;
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect hitbox;
    bool auto_set_size;
};

#endif