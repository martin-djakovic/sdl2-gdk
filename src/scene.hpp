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
    std::vector<BasicSprite *> render_objects;
    std::vector<Text *> ui;
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
        for (int i = 0; i < ui.size(); i++) {
            ui.at(i)->Destroy();
        }

        for (int i = 0; i < render_objects.size(); i++) {
            render_objects.at(i)->Destroy();
        }

        render_objects.clear();
        ui.clear();
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
        render_objects.push_back(sprite);
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
        render_objects.erase(
            std::remove(render_objects.begin(), render_objects.end(), sprite),
            render_objects.end());

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

    void AddUI(Text *text) {
        text->SetRenderer(renderer);
        ui.push_back(text);
    }

    void AddUI(const std::vector<Text *> texts) {
        for (int i = 0; i < texts.size(); i++) {
            texts.at(i)->SetRenderer(renderer);
            this->ui.push_back(texts.at(i));
        }
    }

    void RemoveUI(Text *text, bool destroy = true) {
        ui.erase(std::remove(ui.begin(), ui.end(), text), ui.end());

        if (destroy) {
            text->Destroy();
        }
    }

    void RemoveUI(const std::vector<Text *> texts, bool destroy = true) {
        for (int i = 0; i < texts.size(); i++) {
            ui.erase(std::remove(ui.begin(), ui.end(), texts.at(i)), ui.end());

            if (destroy) {
                texts.at(i)->Destroy();
            }
        }
    }

    // Draw all visible sprites and ui in scene
    void Draw() {
        SDL_RenderClear(renderer);

        SDL_GetRendererOutputSize(renderer, &win_w, &win_h);

        // Draw all render objects (basic sprites)
        for (int i = 0; i < render_objects.size(); i++) {
            if (std::find(camera->GetFocusedSprites()->begin(),
                          camera->GetFocusedSprites()->end(),
                          render_objects.at(i)) ==
                camera->GetFocusedSprites()->end()) {

                render_objects.at(i)->MoveX(camera->x);
                render_objects.at(i)->MoveY(camera->y);
            }

            if (render_objects.at(i)->IsVisible(win_w, win_h)) {
                render_objects.at(i)->Draw();
            }
        }

        // Draw UI
        for (int i = 0; i < ui.size(); i++) {
            ui.at(i)->Draw();
        }

        camera->x = 0;
        camera->y = 0;

        SDL_RenderPresent(renderer);
    }

    std::vector<CollideSprite *> *GetAllCollideSprites() {
        return &collide_sprites;
    }
    std::vector<BasicSprite *> *GetAllBasicSprites() { return &render_objects; }
};

#endif