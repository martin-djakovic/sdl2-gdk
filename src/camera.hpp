#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <iostream>
#include <vector>

#include "basicsprite.hpp"

// Camera affects the view
// All sprites in the scene are offset by camera x and y
// focused_sprites are not offset
class Camera {
  private:
    std::vector<BasicSprite *> focused_sprites;

  public:
    // Camera x gets reset every frame
    double x;
    // Camera y gets reset every frame
    double y;

    Camera() {
        x = 0;
        y = 0;
    }

    Camera(double start_x, double start_y) {
        x = start_x;
        y = start_y;
    }

    Camera(BasicSprite *focused_sprite, double x = 0, double y = 0) {
        focused_sprites.push_back(focused_sprite);
        this->x = x;
        this->y = y;
    }

    Camera(const std::vector<BasicSprite *> focused_sprites, double x = 0,
           double y = 0) {
        this->focused_sprites = focused_sprites;
        this->x = x;
        this->y = y;
    }

    void AddFocusedSprite(BasicSprite *sprite) {
        focused_sprites.push_back(sprite);
    }

    void AddFocusedSprite(const std::vector<BasicSprite *> sprites) {
        for (int i = 0; i < sprites.size(); i++) {
            AddFocusedSprite(sprites.at(i));
        }
    }

    void RemoveFocusedSprite(BasicSprite *sprite) {
        focused_sprites.erase(
            std::remove(focused_sprites.begin(), focused_sprites.end(), sprite),
            focused_sprites.end());
    }

    void RemoveFocusedSprite(const std::vector<BasicSprite *> sprites) {
        for (int i = 0; i < sprites.size(); i++) {
            RemoveFocusedSprite(sprites.at(i));
        }
    }

    void SetFocusedSprites(const std::vector<BasicSprite *> sprites) {
        focused_sprites = sprites;
    }

    std::vector<BasicSprite *> *GetFocusedSprites() { return &focused_sprites; }
};

#endif