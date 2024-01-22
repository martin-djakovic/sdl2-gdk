#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <iostream>
#include <vector>

#include "basicsprite.hpp"

// Camera affects the view
// All objects in the scene are offset by camera x and y
// focused_objects are not offset
class Camera {
  private:
    std::vector<BasicSprite *> focused_objects;

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

    Camera(BasicSprite *focused_object, double x = 0, double y = 0) {
        focused_objects.push_back(focused_object);
        this->x = x;
        this->y = y;
    }

    Camera(const std::vector<BasicSprite *> focused_objects, double x = 0,
           double y = 0) {
        this->focused_objects = focused_objects;
        this->x = x;
        this->y = y;
    }

    void AddFocusedObject(BasicSprite *object) {
        focused_objects.push_back(object);
    }

    void AddFocusedObject(const std::vector<BasicSprite *> object) {
        for (int i = 0; i < object.size(); i++) {
            AddFocusedObject(object.at(i));
        }
    }

    void RemoveFocusedObject(BasicSprite *object) {
        focused_objects.erase(
            std::remove(focused_objects.begin(), focused_objects.end(), object),
            focused_objects.end());
    }

    void RemoveFocusedObject(const std::vector<BasicSprite *> object) {
        for (int i = 0; i < object.size(); i++) {
            RemoveFocusedObject(object.at(i));
        }
    }

    void SetFocusedObjects(const std::vector<BasicSprite *> object) {
        focused_objects = object;
    }

    std::vector<BasicSprite *> *GetFocusedObjects() { return &focused_objects; }
};

#endif