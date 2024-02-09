#include <camera.hpp>

Camera::Camera() {
    x = 0;
    y = 0;
}

Camera::Camera(double start_x, double start_y) {
    x = start_x;
    y = start_y;
}

Camera::Camera(BasicSprite *focused_object, double x, double y) {
    focused_objects.push_back(focused_object);
    this->x = x;
    this->y = y;
}

Camera::Camera(const std::vector<BasicSprite *> focused_objects, double x,
               double y) {
    this->focused_objects = focused_objects;
    this->x = x;
    this->y = y;
}

void Camera::AddFocusedObject(BasicSprite *object) {
    focused_objects.push_back(object);
}

void Camera::AddFocusedObject(const std::vector<BasicSprite *> object) {
    for (int i = 0; i < object.size(); i++) {
        AddFocusedObject(object.at(i));
    }
}

void Camera::RemoveFocusedObject(BasicSprite *object) {
    focused_objects.erase(
        std::remove(focused_objects.begin(), focused_objects.end(), object),
        focused_objects.end());
}

void Camera::RemoveFocusedObjects(const std::vector<BasicSprite *> objects) {
    for (int i = 0; i < objects.size(); i++) {
        RemoveFocusedObject(objects.at(i));
    }
}

void Camera::SetFocusedObjects(const std::vector<BasicSprite *> objects) {
    focused_objects = objects;
}

std::vector<BasicSprite *> *Camera::GetFocusedObjects() {
    return &focused_objects;
}