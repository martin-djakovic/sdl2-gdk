#include "fonttexture.hpp"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <cstdio>
#include <sprite.hpp>

GDK_Sprite::GDK_Sprite() {
  this->x = 0;
  this->y = 0;
  setWidth(0);
  setHeight(0);
}

GDK_Sprite::GDK_Sprite(GDK_ImageTexture *texture) {
  this->x = 0;
  this->y = 0;
  setWidth(0);
  setHeight(0);
  setTexture(texture);
}

GDK_Sprite::GDK_Sprite(GDK_ImageTexture *texture, double x, double y, int width,
                       int height) {
  this->x = x;
  this->y = y;
  setWidth(width);
  setHeight(height);
  setTexture(texture);
}

GDK_Sprite::GDK_Sprite(GDK_AnimatedTexture *texture) {
  this->x = 0;
  this->y = 0;
  setWidth(0);
  setHeight(0);
  setTexture(texture);
}

GDK_Sprite::GDK_Sprite(GDK_AnimatedTexture *texture, double x, double y,
                       int width, int height) {
  this->x = x;
  this->y = y;
  setWidth(width);
  setHeight(height);
  setTexture(texture);
}

GDK_Sprite::GDK_Sprite(GDK_FontTexture *texture) {
  this->x = 0;
  this->y = 0;
  setWidth(0);
  setHeight(0);
  setTexture(texture);
}

GDK_Sprite::GDK_Sprite(GDK_FontTexture *texture, double x, double y) {
  this->x = x;
  this->y = y;
  setTexture(texture);
}

GDK_Sprite::GDK_Sprite(GDK_FontTexture *texture, double x, double y, int width,
                       int height) {
  this->x = x;
  this->y = y;
  setWidth(width);
  setHeight(height);
  setTexture(texture, false);
}

void GDK_Sprite::setPosition(double x, double y) noexcept {
  this->x = x;
  this->y = y;
}

const double GDK_Sprite::getX() noexcept { return x; }
const double GDK_Sprite::getY() noexcept { return y; }

void GDK_Sprite::move(double x, double y) noexcept {
  setPosition(this->x + x, this->y + y);
}

void GDK_Sprite::setWidth(unsigned int width) noexcept {
  if (width == 0) {
    printf(WARN_COLOR "GDK WARNING:" DEF_COLOR " Sprite width is set to 0\n");
  }

  this->width = width;
}

const unsigned int GDK_Sprite::getWidth() noexcept { return width; }

void GDK_Sprite::setHeight(unsigned int height) noexcept {
  if (height == 0) {
    printf(WARN_COLOR "GDK WARNING:" DEF_COLOR " Sprite height is set to 0\n");
  }

  this->height = height;
}

const unsigned int GDK_Sprite::getHeight() noexcept { return height; }

void GDK_Sprite::moveTo(int x, int y, double speed) {
  if (speed == 0) {
    printf(WARN_COLOR "GDK WARNING:" DEF_COLOR
                      " speed parameter of moveTo() is 0, not moving\n");
    return;
  }

  grad_mvmt_goalx = x;
  grad_mvmt_goaly = y;

  int xdif = this->x - x;
  int ydif = this->y - y;
  // Movement iterations along line between this position and given
  // position
  grad_mvmt_iter = sqrt(xdif * xdif + ydif * ydif) / speed;

  if (grad_mvmt_iter == 0) {
    return;
  }

  grad_mvmt_speedx = -((double)xdif / grad_mvmt_iter);
  grad_mvmt_speedy = -((double)ydif / grad_mvmt_iter);
}

void GDK_Sprite::gradualMovementIterator() {
  if (grad_mvmt_iter > 0) {
    move(grad_mvmt_speedx, grad_mvmt_speedy);

    grad_mvmt_iter--;

    if (grad_mvmt_iter == 0 && x != grad_mvmt_goalx && y != grad_mvmt_goalx) {
      setPosition(grad_mvmt_goalx, grad_mvmt_goaly);
    }
  }
}

void GDK_Sprite::setTexture(GDK_ImageTexture *texture) {
  // Print error if renderer is null
  if (texture->renderer == nullptr) {
    printf(ERR_COLOR
           "GDK ERROR:" DEF_COLOR
           " Failed setting sprite texture - texture renderer not set\n");
    return;
  }

  // Print error if texture is null
  if (texture->sdl_texture == nullptr) {
    printf(ERR_COLOR "GDK ERROR:" DEF_COLOR
                     " Failed setting sprite texture - texture not loaded\n");
    return;
  }

  this->texture = texture;
}

void GDK_Sprite::setTexture(GDK_AnimatedTexture *texture) {
  // Print error if renderer is null
  if (texture->renderer == nullptr) {
    printf(ERR_COLOR
           "GDK ERROR:" DEF_COLOR
           " Failed setting sprite texture - texture renderer not set\n");
    return;
  }

  // Print error if texture is null
  if (texture->sdl_texture == nullptr) {
    printf(ERR_COLOR "GDK ERROR:" DEF_COLOR
                     " Failed setting sprite texture - texture not loaded\n");
    return;
  }

  this->texture = texture;
}

void GDK_Sprite::setTexture(GDK_FontTexture *texture,
                            const bool auto_set_size) {
  // Print error if texture renderer is null
  if (texture->renderer == nullptr) {
    printf(ERR_COLOR
           "GDK ERROR:" DEF_COLOR
           " Failed setting sprite texture - texture renderer not set\n");
    return;
  }

  // Print error if texture is null and we're trying to automatically fit sprite
  // size to texture size
  if (texture->sdl_texture == nullptr && auto_set_size) {
    printf(ERR_COLOR "GDK ERROR:" DEF_COLOR
                     " Failed setting sprite size - texture not loaded\n");
    return;
  }

  // Print warning if texture is null
  if (texture->sdl_texture == nullptr) {
    printf(ERR_COLOR "GDK ERROR:" DEF_COLOR
                     " Failed setting sprite texture - texture not loaded\n");
    return;
  }

  this->texture = texture;

  if (auto_set_size) {
    SDL_QueryTexture(texture->sdl_texture, NULL, NULL, &width, &height);
  }
}

void GDK_Sprite::setFlip(SDL_RendererFlip flip) { this->flip = flip; }

void GDK_Sprite::setRotation(double angle) noexcept { rotation_angle = angle; }

void GDK_Sprite::rotate(double angle) noexcept {
  setRotation(rotation_angle + angle);
}

void GDK_Sprite::setRotationCenter(SDL_Point *rotation_point) {
  this->rotation_point = rotation_point;
}

void GDK_Sprite::draw() {
  rect.x = x;
  rect.y = y;
  rect.w = width;
  rect.h = height;

  // Complete step of gradual movement on every frame (when sprite is
  // drawn)
  gradualMovementIterator();

  // Check if texture was set
  if (texture == nullptr) {
    printf(ERR_COLOR "GDK ERROR:" DEF_COLOR
                     " Failed drawing sprite - texture not set\n");
    return;
  }

  // Check if texture was loaded
  if (texture->sdl_texture == nullptr) {
    printf(ERR_COLOR "GDK ERROR:" DEF_COLOR
                     " Failed drawing sprite - texture not loaded\n");
    return;
  }

  // Check if renderer is set properly
  if (texture->renderer == nullptr) {
    printf(ERR_COLOR "GDK ERROR:" DEF_COLOR
                     " Failed drawing sprite - texture renderer not set\n");
    return;
  }

  // Print warning if sprite texture is not visible (width/height is 0)
  if (width == 0 || height == 0) {
    printf(WARN_COLOR "GDK WARNING:" DEF_COLOR
                      " Sprite drawn but not visible - width/height is 0\n");
  }
  // Print warning if sprite texture is not visible (out of bounds)
  if (!isInBounds()) {
    printf(WARN_COLOR "GDK WARNING:" DEF_COLOR
                      " Sprite drawn but not visible - out of bounds\n");
  }

  texture->flagDrawEvent();

  SDL_RenderCopyEx(texture->renderer, texture->sdl_texture, NULL, &rect,
                   rotation_angle, rotation_point, flip);
}

const bool GDK_Sprite::isInBounds() {
  int win_w, win_h;

  if (texture == nullptr) {
    printf(ERR_COLOR
           "GDK ERROR:" DEF_COLOR
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
