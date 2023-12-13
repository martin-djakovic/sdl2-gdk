#ifndef SCENE_HPP
#define SCENE_HPP

#include <algorithm>
#include <iostream>
#include <vector>

#include "collidesprite.hpp"
#include "text.hpp"
#include "camera.hpp"

// Scene is used to group multiple sprites and draw them to the screen
// Only one scene at a time should be drawn in any single window
class Scene {
  private:
    std::vector<CollideSprite *> collide_sprites;
    std::vector<BasicSprite *> basic_sprites;
    int win_w;
    int win_h;

  public:
    SDL_Renderer *renderer;
    Camera *camera;

    Scene(SDL_Window *window, Camera *camera) {
        renderer = SDL_GetRenderer(window);
        this->camera = camera;
    }

    void Destroy() {
        for (int i = 0; i < basic_sprites.size(); i++) {
            basic_sprites.at(i)->Destroy();
        }

        basic_sprites.clear();
        collide_sprites.clear();
    }

    void AddCollideSprite(CollideSprite *sprite) {
        sprite->SetColliders(&collide_sprites);        
        collide_sprites.push_back(sprite);
        AddBasicSprite(sprite);
    }

    void AddCollideSprite(const std::vector<CollideSprite *> sprites) {
        for (int i = 0; i < sprites.size(); i++) {
            AddCollideSprite(sprites.at(i));
        }
    }

    void AddBasicSprite(BasicSprite *sprite) {
        sprite->SetRenderer(renderer);
        basic_sprites.push_back(sprite);
    }

    void AddBasicSprite(const std::vector<BasicSprite *> sprites) {
        for (int i = 0; i < sprites.size(); i++) {
            AddBasicSprite(sprites.at(i));
        }
    }

    void RemoveCollideSprite(CollideSprite *sprite, bool destroy = true) {
        collide_sprites.erase(
            std::remove(collide_sprites.begin(), collide_sprites.end(), sprite),
            collide_sprites.end());

        RemoveBasicSprite(sprite, destroy);
    }

    void RemoveCollideSprite(const std::vector<CollideSprite *> sprites,
                      bool destroy = true) {
        for (int i = 0; i < sprites.size(); i++) {
            RemoveCollideSprite(sprites.at(i));
        }
    }

    void RemoveBasicSprite(BasicSprite *sprite, bool destroy = true) {
        basic_sprites.erase(
            std::remove(basic_sprites.begin(), basic_sprites.end(), sprite),
            basic_sprites.end());

        if (destroy) {
            sprite->Destroy();
        }
    }

    void RemoveBasicSprite(const std::vector<BasicSprite *> sprites,
                      bool destroy = true) {
        for (int i = 0; i < sprites.size(); i++) {
            RemoveBasicSprite(sprites.at(i));
        }
    }

    // Draw all visible sprites in scene
    void Draw() {
        SDL_RenderClear(renderer);

        SDL_GetRendererOutputSize(renderer, &win_w, &win_h);

        // Draw all basic sprites
        for (int i = 0; i < basic_sprites.size(); i++) {
            if (std::find(camera->GetFocusedSprites()->begin(),
                          camera->GetFocusedSprites()->end(),
                          basic_sprites.at(i)) ==
                camera->GetFocusedSprites()->end()) {

                basic_sprites.at(i)->MoveX(camera->x);
                basic_sprites.at(i)->MoveY(camera->y);
            }

            if (basic_sprites.at(i)->IsVisible(win_w, win_h)) {
                basic_sprites.at(i)->Draw();
            }
        }

        camera->x = 0;
        camera->y = 0;

        SDL_RenderPresent(renderer);
    }

    std::vector<CollideSprite *> *GetAllCollideSprites() {
        return &collide_sprites;
    }
    std::vector<BasicSprite *> *GetAllBasicSprites() { return &basic_sprites; }
};

#endif