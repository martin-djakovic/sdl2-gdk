#ifndef SCENE_HPP
#define SCENE_HPP

#include <algorithm>
#include <camera.hpp>
#include <collidesprite.hpp>
#include <iostream>
#include <textblock.hpp>
#include <textline.hpp>
#include <vector>

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

    // To calculate FPS, we need 2 frames to know the time between them,
    // odd_frame keeps track if the calculation is on the first or second frame
    bool odd_frame = true;
    Uint64 last_frame_time;
    int fps;

    bool show_hitbox_outlines = false;
    SDL_Color hitbox_outline_color = {0, 255, 0};

    SDL_Renderer *renderer;
    Camera *camera;

    void AddRenderObject(BasicSprite *object);

    void RemoveRenderObject(BasicSprite *object, bool destroy = true);

    // Calculate current frames per second
    // Must be called on every draw
    void CalculateFPS();

    // Draw hitbox outlines of all collide sprites in scene
    void DrawHitboxOutlines();

    void MoveCamera();

    // Draw everything in the scene
    void DrawRenderObjects();

  public:
    Scene(SDL_Window *window, Camera *camera);

    void Destroy();

    void AddCollideObject(CollideSprite *collide_object);

    void AddCollideObject(const std::vector<CollideSprite *> collide_object);

    void AddObject(BasicSprite *object);

    void AddObject(const std::vector<BasicSprite *> object);

    void RemoveCollideObject(CollideSprite *collide_object,
                             bool destroy = true);

    void RemoveCollideObject(const std::vector<CollideSprite *> collide_object,
                             bool destroy = true);

    void RemoveObject(BasicSprite *object, bool destroy = true);

    void RemoveObject(const std::vector<BasicSprite *> object,
                      bool destroy = true);

    // Draw all visible objects in scene
    void Draw();

    std::vector<CollideSprite *> *GetAllCollideObjects();

    std::vector<BasicSprite *> *GetAllObjects();

    int GetFPS();

    // Set if hitbox outlines of all collide objects should be drawn
    // Enabling hitbox outlines will negatively impact performance, only use
    // when debugging
    void SetShowHitboxOutlines(bool show_hitbox_outlines);

    // Set color of hitbox outlines
    void SetHitboxOutlineColor(SDL_Color color);
};

#endif