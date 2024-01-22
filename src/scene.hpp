#ifndef SCENE_HPP
#define SCENE_HPP

#include <algorithm>
#include <iostream>
#include <vector>

#include "camera.hpp"
#include "collidesprite.hpp"
#include "debug.hpp"
#include "textblock.hpp"
#include "textline.hpp"

// Scene is used to group multiple sprites and draw them to the screen
// Only one scene at a time should be drawn in any single window
class Scene {
  private:
    std::vector<CollideSprite *> collide_sprites;
    std::vector<BasicSprite *> basic_sprites;
    // Every object that should be drawn in the scene
    std::vector<BasicSprite *> render_objects;
    int win_w;
    int win_h;

    SDL_Renderer *renderer;
    Camera *camera;

    void AddRenderObject(BasicSprite *object) {
        object->SetRenderer(renderer);
        render_objects.push_back(object);
    }

    void RemoveRenderObject(BasicSprite *object, bool destroy = true) {
        render_objects.erase(
            std::remove(render_objects.begin(), render_objects.end(), object),
            render_objects.end());

        if (destroy) {
            object->Destroy();
        }
    }

  public:
    Scene(SDL_Window *window, Camera *camera) {
        renderer = SDL_GetRenderer(window);
        this->camera = camera;
    }

    void Destroy() {
        for (int i = 0; i < render_objects.size(); i++) {
            render_objects.at(i)->Destroy();
        }

        basic_sprites.clear();
        collide_sprites.clear();
        render_objects.clear();
    }

    void AddCollideObject(CollideSprite *collide_object) {
        AddRenderObject(collide_object);
        collide_object->SetColliders(&collide_sprites);
        collide_sprites.push_back(collide_object);
    }

    void AddCollideObject(const std::vector<CollideSprite *> collide_object) {
        for (int i = 0; i < collide_object.size(); i++) {
            AddCollideObject(collide_object.at(i));
        }
    }

    void AddObject(BasicSprite *object) {
        AddRenderObject(object);
        basic_sprites.push_back(object);
    }

    void AddObject(const std::vector<BasicSprite *> object) {
        for (int i = 0; i < object.size(); i++) {
            AddObject(object.at(i));
        }
    }

    void RemoveCollideObject(CollideSprite *collide_object, bool destroy = true) {
        collide_sprites.erase(
            std::remove(collide_sprites.begin(), collide_sprites.end(), collide_object),
            collide_sprites.end());

        RemoveRenderObject(collide_object, destroy);
    }

    void RemoveCollideObject(const std::vector<CollideSprite *> collide_object,
                             bool destroy = true) {
        for (int i = 0; i < collide_object.size(); i++) {
            RemoveCollideObject(collide_object.at(i));
        }
    }

    void RemoveObject(BasicSprite *object, bool destroy = true) {
        basic_sprites.erase(
            std::remove(basic_sprites.begin(), basic_sprites.end(), object),
            basic_sprites.end());
        RemoveRenderObject(object, destroy);
    }

    void RemoveObject(const std::vector<BasicSprite *> object,
                           bool destroy = true) {
        for (int i = 0; i < object.size(); i++) {
            RemoveObject(object.at(i));
        }
    }

    // Draw all visible objects in scene
    void Draw() {
        SDL_RenderClear(renderer);

        SDL_GetRendererOutputSize(renderer, &win_w, &win_h);

        // Move camera for all objects
        for (int i = 0; i < basic_sprites.size(); i++) {
            // Make sure that object isn't a focused sprite,
            // and that camera has been moved
            if (std::find(camera->GetFocusedObjects()->begin(),
                          camera->GetFocusedObjects()->end(),
                          basic_sprites.at(i)) ==
                    camera->GetFocusedObjects()->end() &&
                (camera->x != 0 || camera->y != 0)) {

                basic_sprites.at(i)->MoveX(camera->x);
                basic_sprites.at(i)->MoveY(camera->y);
            }
        }

        // Move camera for all collide sprites
        for (int i = 0; i < collide_sprites.size(); i++) {
            // Make sure that collide object isn't a focused sprite,
            // and that camera has been moved
            if (std::find(camera->GetFocusedObjects()->begin(),
                          camera->GetFocusedObjects()->end(),
                          collide_sprites.at(i)) ==
                    camera->GetFocusedObjects()->end() &&
                (camera->x != 0 || camera->y != 0)) {

                collide_sprites.at(i)->MoveX(camera->x, false);
                collide_sprites.at(i)->MoveY(camera->y, false);
            }
        }

        // Draw everything (all render objects)
        for (int i = 0; i < render_objects.size(); i++) {
            render_objects.at(i)->Draw();
        }

        camera->x = 0;
        camera->y = 0;

        SDL_RenderPresent(renderer);
    }

    std::vector<CollideSprite *> *GetAllCollideObjects() {
        return &collide_sprites;
    }
    std::vector<BasicSprite *> *GetAllObjects() { return &render_objects; }
};

#endif