#ifndef SCENE_HPP
#define SCENE_HPP

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
    Sprite *focused_sprite;

    Camera(Sprite *focused_sprite, int start_x = 0, int start_y = 0) {
        this->focused_sprite = focused_sprite;
        x = start_x;
        y = start_y;
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
            if (camera->focused_sprite != sprites.at(i)) {
                sprites.at(i)->SetX(sprites.at(i)->GetX() + camera->x, false);
                sprites.at(i)->SetY(sprites.at(i)->GetY() + camera->y, false);
            }

            if (sprites.at(i)->IsVisible(win_w, win_h)) {
                sprites.at(i)->Draw();
            }
        }

        camera->x = 0;
        camera->y = 0;

        SDL_RenderPresent(renderer);
    }

    std::vector<Sprite *> *GetAllSprites() {
        return &sprites;
    }

  private:
    std::vector<Sprite *> sprites;
    int win_w;
    int win_h;
};

#endif