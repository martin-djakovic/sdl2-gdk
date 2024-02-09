#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <basicsprite.hpp>

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

    Camera();

    Camera(double start_x, double start_y);

    Camera(BasicSprite *focused_object, double x = 0, double y = 0);

    Camera(const std::vector<BasicSprite *> focused_objects, double x = 0,
           double y = 0);

    void AddFocusedObject(BasicSprite *object);

    void AddFocusedObject(const std::vector<BasicSprite *> object);

    void RemoveFocusedObject(BasicSprite *object);

    void RemoveFocusedObjects(const std::vector<BasicSprite *> objects);

    void SetFocusedObjects(const std::vector<BasicSprite *> objects);

    std::vector<BasicSprite *> *GetFocusedObjects();
};

#endif