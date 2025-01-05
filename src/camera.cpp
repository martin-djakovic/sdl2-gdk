#include <camera.hpp>

namespace gdk {
Camera::Camera() {}

Camera::Camera(const std::vector<Sprite *> focused_sprites) {
  this->focused_sprites = focused_sprites;
}

void Camera::move(double x, double y) noexcept {
  offset_x = x;
  offset_y = y;
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
