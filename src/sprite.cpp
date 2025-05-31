#ifdef __linux__
#include <sdl2-gdk/fonttexture.hpp>
#include <sdl2-gdk/sprite.hpp>

#elif _WIN32
#include <fonttexture.hpp>
#include <sprite.hpp>

#elif __APPLE__
#include <fonttexture.hpp>
#include <sprite.hpp>

#else
#error "FATAL ERROR: Operating system not supported"

#endif

#include <cstdio>

namespace gdk {
Sprite::Sprite() {
  this->x = 0;
  this->y = 0;
  setWidth(0);
  setHeight(0);
}

Sprite::Sprite(ImageTexture *texture, int zindex) {
  this->x = 0;
  this->y = 0;
  setWidth(0);
  setHeight(0);
  setTexture(texture);
  setZindex(zindex);
}

Sprite::Sprite(ImageTexture *texture, double x, double y, int width, int height,
               int zindex) {
  this->x = x;
  this->y = y;
  setWidth(width);
  setHeight(height);
  setTexture(texture);
  setZindex(zindex);
}

Sprite::Sprite(AnimatedTexture *texture, int zindex) {
  this->x = 0;
  this->y = 0;
  setWidth(0);
  setHeight(0);
  setTexture(texture);
  setZindex(zindex);
}

Sprite::Sprite(AnimatedTexture *texture, double x, double y, int width,
               int height, int zindex) {
  this->x = x;
  this->y = y;
  setWidth(width);
  setHeight(height);
  setTexture(texture);
  setZindex(zindex);
}

Sprite::Sprite(FontTexture *texture, int zindex) {
  this->x = 0;
  this->y = 0;
  setWidth(0);
  setHeight(0);
  setTexture(texture);
  setZindex(zindex);
}

Sprite::Sprite(FontTexture *texture, double x, double y, int zindex) {
  this->x = x;
  this->y = y;
  setTexture(texture);
  setZindex(zindex);
}

Sprite::Sprite(FontTexture *texture, double x, double y, int width, int height,
               int zindex) {
  this->x = x;
  this->y = y;
  setWidth(width);
  setHeight(height);
  setTexture(texture, false);
  setZindex(zindex);
}

Sprite::Sprite(Sprite *sprite) {
  x = sprite->x;
  y = sprite->y;
  zindex = sprite->zindex;
  width = sprite->width;
  height = sprite->height;
  rect = sprite->rect;
  texture = sprite->texture;
  flip = sprite->flip;
  rotation_point = sprite->rotation_point;
  rotation_angle = sprite->rotation_angle;
}

Sprite::Sprite(Sprite *sprite, double x, double y) {
  this->x = x;
  this->y = y;
  zindex = sprite->zindex;
  width = sprite->width;
  height = sprite->height;
  rect = sprite->rect;
  texture = sprite->texture;
  flip = sprite->flip;
  rotation_point = sprite->rotation_point;
  rotation_angle = sprite->rotation_angle;
}

const bool Sprite::comparePtr(Sprite *s1, Sprite *s2) { return *s1 < *s2; }

void Sprite::setPosition(double x, double y) noexcept {
  this->x = x;
  this->y = y;
}

const double Sprite::getX() noexcept { return x; }
const double Sprite::getY() noexcept { return y; }

void Sprite::move(double x, double y) noexcept {
  setPosition(this->x + x, this->y + y);
}

void Sprite::setWidth(unsigned int width) noexcept {
  if (width == 0) {
    printf(GDK_WARNING_COLOR "GDK WARNING:" GDK_DEFAULT_COLOR
                             " Sprite width is set to 0\n");
  }

  this->width = width;
}

const unsigned int Sprite::getWidth() noexcept { return width; }

void Sprite::setHeight(unsigned int height) noexcept {
  if (height == 0) {
    printf(GDK_WARNING_COLOR "GDK WARNING:" GDK_DEFAULT_COLOR
                             " Sprite height is set to 0\n");
  }

  this->height = height;
}

const unsigned int Sprite::getHeight() noexcept { return height; }

void Sprite::setTexture(ImageTexture *texture) {
  // Print error if renderer is null
  if (texture->renderer == nullptr) {
    printf(GDK_ERROR_COLOR
           "GDK ERROR:" GDK_DEFAULT_COLOR
           " Failed setting sprite texture - texture renderer not set\n");
    return;
  }

  // Print error if texture is null
  if (texture->sdl_texture == nullptr) {
    printf(GDK_ERROR_COLOR
           "GDK ERROR:" GDK_DEFAULT_COLOR
           " Failed setting sprite texture - texture not loaded\n");
    return;
  }

  this->texture = texture;
}

void Sprite::setTexture(AnimatedTexture *texture) {
  // Print error if renderer is null
  if (texture->renderer == nullptr) {
    printf(GDK_ERROR_COLOR
           "GDK ERROR:" GDK_DEFAULT_COLOR
           " Failed setting sprite texture - texture renderer not set\n");
    return;
  }

  // Print error if texture is null
  if (texture->sdl_texture == nullptr) {
    printf(GDK_ERROR_COLOR
           "GDK ERROR:" GDK_DEFAULT_COLOR
           " Failed setting sprite texture - texture not loaded\n");
    return;
  }

  this->texture = texture;
}

void Sprite::setTexture(FontTexture *texture, const bool auto_set_size) {
  // Print error if texture renderer is null
  if (texture->renderer == nullptr) {
    printf(GDK_ERROR_COLOR
           "GDK ERROR:" GDK_DEFAULT_COLOR
           " Failed setting sprite texture - texture renderer not set\n");
    return;
  }

  // Print error if texture is null and we're trying to automatically fit sprite
  // size to texture size
  if (texture->sdl_texture == nullptr && auto_set_size) {
    printf(GDK_ERROR_COLOR
           "GDK ERROR:" GDK_DEFAULT_COLOR
           " Failed setting sprite size - texture not loaded\n");
    return;
  }

  // Print warning if texture is null
  if (texture->sdl_texture == nullptr) {
    printf(GDK_ERROR_COLOR
           "GDK ERROR:" GDK_DEFAULT_COLOR
           " Failed setting sprite texture - texture not loaded\n");
    return;
  }

  this->texture = texture;

  if (auto_set_size) {
    SDL_QueryTexture(texture->sdl_texture, NULL, NULL, &width, &height);
  }
}

void Sprite::setFlip(SDL_RendererFlip flip) { this->flip = flip; }

void Sprite::setZindex(int zindex) { this->zindex = zindex; }

void Sprite::setRotation(double angle) noexcept { rotation_angle = angle; }

void Sprite::rotate(double angle) noexcept {
  setRotation(rotation_angle + angle);
}

void Sprite::setRotationCenter(SDL_Point *rotation_point) {
  this->rotation_point = rotation_point;
}

void Sprite::draw() {
  rect.x = x;
  rect.y = y;
  rect.w = width;
  rect.h = height;

  // Check if texture was set
  if (texture == nullptr) {
    printf(GDK_ERROR_COLOR "GDK ERROR:" GDK_DEFAULT_COLOR
                           " Failed drawing sprite - texture not set\n");
    return;
  }

  // Check if texture was loaded
  if (texture->sdl_texture == nullptr) {
    printf(GDK_ERROR_COLOR "GDK ERROR:" GDK_DEFAULT_COLOR
                           " Failed drawing sprite - texture not loaded\n");
    return;
  }

  // Check if renderer is set properly
  if (texture->renderer == nullptr) {
    printf(GDK_ERROR_COLOR
           "GDK ERROR:" GDK_DEFAULT_COLOR
           " Failed drawing sprite - texture renderer not set\n");
    return;
  }

  // Print warning if sprite texture is not visible (width/height is 0)
  if (width == 0 || height == 0) {
    printf(GDK_WARNING_COLOR
           "GDK WARNING:" GDK_DEFAULT_COLOR
           " Sprite drawn but not visible - width/height is 0\n");
  }
  // Print warning if sprite texture is not visible (out of bounds)
  if (!isInBounds()) {
    printf(GDK_WARNING_COLOR "GDK WARNING:" GDK_DEFAULT_COLOR
                             " Sprite drawn but not visible - out of bounds\n");
  }

  texture->flagDrawEvent();

  SDL_RenderCopyEx(texture->renderer, texture->sdl_texture, NULL, &rect,
                   rotation_angle, rotation_point, flip);
}

const bool Sprite::isInBounds() {
  int win_w, win_h;

  if (texture == nullptr) {
    printf(GDK_ERROR_COLOR
           "GDK ERROR:" GDK_DEFAULT_COLOR
           " Cannot check if sprite is in bounds - texture not set\n");
    return false;
  }

  SDL_GetRendererOutputSize(texture->renderer, &win_w, &win_h);

  if (x + width > 0 && y + height > 0 && x < win_w && y < win_h) {
    return true;
  } else {
    return false;
  }
}

const bool Sprite::operator<(const Sprite &other_sprite) {
  return zindex < other_sprite.zindex;
}

const bool Sprite::operator>(const Sprite &other_sprite) {
  return zindex > other_sprite.zindex;
}
} // namespace gdk