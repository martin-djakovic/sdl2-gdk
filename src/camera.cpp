#ifdef __linux__
#include <sdl2-gdk/camera.hpp>

#elif _WIN32
#include <camera.hpp>

#elif __APPLE__
#include <camera.hpp>

#else
#error "FATAL ERROR: Operating system not supported"

#endif

#include <cstdio>

namespace gdk {
Camera::Camera() {}

Camera::Camera(const std::vector<Sprite *> focused_sprites) {
  setFocusedSprites(focused_sprites);
}

Camera::Camera(Camera *camera) {
  setFocusedSprites(camera->focused_sprites);
  attachScene(camera->scene);
}

Camera::Camera(Camera *camera, Scene *Scene) {
  setFocusedSprites(camera->focused_sprites);
  attachScene(scene);
}

void Camera::attachScene(Scene *scene) noexcept { this->scene = scene; }

void Camera::detachScene() noexcept { scene = nullptr; }

void Camera::move(double x, double y) {
  if (scene == nullptr) {
    printf(GDK_ERROR_COLOR "GDK ERROR:" GDK_DEFAULT_COLOR " No scene attached to camera\n");
    return;
  }

  if (x == 0 && y == 0) {
    return;
  }

  std::vector<Sprite *> *basic_sprites = scene->getBasicSprites();
  std::vector<CollideSprite *> *collide_sprites = scene->getCollideSprites();

  // Move camera for all basic sprites
  for (int i = 0; i < basic_sprites->size(); i++) {
    // Make sure that sprite isn't a focused sprite,
    // and that camera has been moved
    if (std::find(focused_sprites.begin(), focused_sprites.end(),
                  basic_sprites->at(i)) == focused_sprites.end() &&
        (x != 0 || y != 0)) {

      basic_sprites->at(i)->move(x, 0);
      basic_sprites->at(i)->move(0, y);
    }
  }

  // Move camera for all collide sprites
  for (int i = 0; i < collide_sprites->size(); i++) {
    // Make sure that collide sprite isn't a focused sprite,
    // and that camera has been moved
    if (std::find(focused_sprites.begin(), focused_sprites.end(),
                  collide_sprites->at(i)) == focused_sprites.end() &&
        (x != 0 || y != 0)) {

      // We need to move collide sprite without the use of move() function in
      // this case, in order to avoid sprites colliding during camera movement
      collide_sprites->at(i)->x += x;
      collide_sprites->at(i)->y += y;
      collide_sprites->at(i)->updateHitboxCoords();
    }
  }

  x = 0;
  y = 0;
}

void Camera::addFocusedSprite(Sprite *sprite) {
  focused_sprites.push_back(sprite);
}

void Camera::addFocusedSprite(const std::vector<Sprite *> sprites) {
  for (int i = 0; i < sprites.size(); i++) {
    addFocusedSprite(sprites.at(i));
  }
}

void Camera::removeFocusedSprite(Sprite *sprite) {
  focused_sprites.erase(
      std::remove(focused_sprites.begin(), focused_sprites.end(), sprite),
      focused_sprites.end());
}

void Camera::removeFocusedSprite(const std::vector<Sprite *> sprites) {
  for (int i = 0; i < sprites.size(); i++) {
    removeFocusedSprite(sprites.at(i));
  }
}

void Camera::setFocusedSprites(const std::vector<Sprite *> sprites) {
  focused_sprites = sprites;
}

std::vector<Sprite *> *Camera::getFocusedSprites() noexcept {
  return &focused_sprites;
}
} // namespace gdk