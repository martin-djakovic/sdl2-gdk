#ifndef SCENE_HPP
#define SCENE_HPP

#include <algorithm>
#include <iostream>
#include <vector>

#include "sprite.hpp"

// Camera affects the view
// All sprites in the scene are offset by camera x and y
// focused_sprite is not offset
// Pass nullptr as focused_sprite if you want all sprites in scene to be
// affected by the camera
class Camera {
  public:
    // Camera x gets reset every frame
    int x;
    // Camera y gets reset every frame
    int y;
    std::vector<Sprite *> focused_sprites;

    Camera() {
        x = 0;
        y = 0;
    }

    Camera(int start_x, int start_y) {
        x = start_x;
        y = start_y;
    }

    Camera(Sprite *focused_sprite, int x = 0, int y = 0) {
        focused_sprites.push_back(focused_sprite);
        this->x = x;
        this->y = y;
    }

    Camera(std::vector<Sprite *> focused_sprites, int x = 0, int y = 0) {
        this->focused_sprites = focused_sprites;
        this->x = x;
        this->y = y;
    }

    Camera(std::vector<Sprite *> *focused_sprites, int x = 0, int y = 0) {
        this->focused_sprites = *focused_sprites;
        this->x = x;
        this->y = y;
    }
};

// Scene is used to group multiple sprites and draw them to the screen
// Only one scene at a time should be drawn in any single window
class Scene {

  public:
    SDL_Renderer *renderer;
    Camera *camera;

    Scene(SDL_Window *window, int index, Uint32 flags, Camera *camera) {
        renderer = SDL_CreateRenderer(window, index, flags);
        this->camera = camera;
    }

    void Destroy() {
        SDL_DestroyRenderer(renderer);
        sprites.clear();
    }

    void Add(Sprite *sprite) {
        sprite->SetColliders(&sprites);
        sprite->SetRenderer(renderer);
        sprites.push_back(sprite);
    }

    // Draw all visible sprites in the scene
    void Draw() {
        SDL_RenderClear(renderer);

        SDL_GetRendererOutputSize(renderer, &win_w, &win_h);

        for (int i = 0; i < sprites.size(); i++) {
            if (std::find(camera->focused_sprites.begin(),
                          camera->focused_sprites.end(),
                          sprites.at(i)) == camera->focused_sprites.end()) {

                sprites.at(i)->MoveX(camera->x, false);
                sprites.at(i)->MoveY(camera->y, false);
            }

            if (sprites.at(i)->IsVisible(win_w, win_h)) {
                sprites.at(i)->Draw();
            }
        }

        camera->x = 0;
        camera->y = 0;

        SDL_RenderPresent(renderer);
    }

    std::vector<Sprite *> *GetAllSprites() { return &sprites; }

  private:
    std::vector<Sprite *> sprites;
    int win_w;
    int win_h;
};

#endif