#include <algorithm>
#include <sdl2-gdk/collidesprite.hpp>
#include <cstdio>
#include <sdl2-gdk/texture.hpp>

namespace gdk {
CollideSprite::CollideSprite() : Sprite() { setHitboxProperties(0, 0, 0, 0); }

CollideSprite::CollideSprite(ImageTexture *texture, int zindex)
    : Sprite(texture, zindex) {
  setHitboxProperties(0, 0, 0, 0);
}

CollideSprite::CollideSprite(AnimatedTexture *texture, int zindex)
    : Sprite(texture, zindex) {
  setHitboxProperties(0, 0, 0, 0);
}

CollideSprite::CollideSprite(AnimatedTexture *texture, double x, double y,
                             int width, int height, int zindex)
    : Sprite(texture, x, y, width, height, zindex) {
  setHitboxProperties(0, 0, width, height);
}

CollideSprite::CollideSprite(ImageTexture *texture, double x, double y,
                             int width, int height, int zindex)
    : Sprite(texture, x, y, width, height, zindex) {
  setHitboxProperties(0, 0, width, height);
}

CollideSprite::CollideSprite(CollideSprite *sprite) : Sprite(sprite) {
  hitbox = sprite->hitbox;
  hitbox_xoffset = sprite->hitbox_xoffset;
  hitbox_yoffset = sprite->hitbox_yoffset;
  enable_movement_collision = sprite->enable_movement_collision;
}

CollideSprite::CollideSprite(CollideSprite *sprite, double x, double y)
    : Sprite(sprite, x, y) {
  hitbox = sprite->hitbox;
  hitbox_xoffset = sprite->hitbox_xoffset;
  hitbox_yoffset = sprite->hitbox_yoffset;
  enable_movement_collision = sprite->enable_movement_collision;
}

const bool CollideSprite::movementCollided(CollideSprite *collide_sprite) {
  int collider_hbx = collide_sprite->getHitbox()->x;
  int collider_hby = collide_sprite->getHitbox()->y;
  int collider_hbw = collide_sprite->getHitbox()->w;
  int collider_hbh = collide_sprite->getHitbox()->h;

  // Check if sprite overlaps during movement
  bool movement_collides = hitbox.x + hitbox.w > collider_hbx &&
                           hitbox.x < collider_hbx + collider_hbw &&
                           hitbox.y + hitbox.h > collider_hby &&
                           hitbox.y < collider_hby + collider_hbh;

  if (movement_collides && collide_sprite->enable_movement_collision) {
    return true;
  } else {
    return false;
  }
}

void CollideSprite::updateHitboxCoords() noexcept {
  hitbox.x = x + hitbox_xoffset;
  hitbox.y = y + hitbox_yoffset;
}

void CollideSprite::setEnableMovementCollision(
    bool enable_movement_collision) noexcept {
  this->enable_movement_collision = enable_movement_collision;
}

const bool CollideSprite::getEnableMovementCollision() noexcept {
  return enable_movement_collision;
}

void CollideSprite::setPosition(double x, double y) noexcept {
  // TODO:
  // Make this function less abysmal when you have time
  bool collision_detected = false;
  double old_y = this->y;

  // Check if sprite collides at new position
  for (int i = 0; i < colliders->size(); i++) {
    double collider_x = colliders->at(i)->getHitbox()->x;
    double collider_y = colliders->at(i)->getHitbox()->y;
    double collider_w = colliders->at(i)->getHitbox()->w;
    double collider_h = colliders->at(i)->getHitbox()->h;

    this->x = x;
    this->y = old_y;
    updateHitboxCoords();

    if (colliders->at(i) != this && movementCollided(colliders->at(i))) {

      // Adjust sprite x position if collided
      if (hitbox.x < collider_x) {
        this->x = collider_x - hitbox.w - hitbox_xoffset;
      } else {
        this->x = collider_x + collider_w - hitbox_xoffset;
      }

      collision_detected = true;
      updateHitboxCoords();
    }

    this->y = y;
    updateHitboxCoords();

    if (colliders->at(i) != this && movementCollided(colliders->at(i))) {
      // Adjust sprite y position if collided
      if (hitbox.y < collider_y) {
        this->y = collider_y - hitbox.h - hitbox_yoffset;
      } else {
        this->y = collider_y + collider_h - hitbox_yoffset;
      }

      collision_detected = true;
      updateHitboxCoords();
    }

    if (collision_detected) {
      break;
    }
  }
}

void CollideSprite::move(double x, double y) noexcept {
  setPosition(this->x + x, this->y + y);
}

void CollideSprite::setColliders(std::vector<CollideSprite *> *colliders) {
  this->colliders = colliders;
}

void CollideSprite::setHitboxProperties(double x_offset, double y_offset,
                                        int width, int height) {
  // Print warning if hitbox width/height is 0, because then collisions can't
  // be checked
  if (width == 0 || height == 0) {
    printf(GDK_WARNING_COLOR "GDK WARNING:" GDK_DEFAULT_COLOR
                      " Hitbox width/height of CollideSprite set to 0 "
                      "- will be unable to perform collisions\n");
  }

  // Print error if hitbox width/height is set to less than 0
  if (width < 0 || height < 0) {
    printf(GDK_ERROR_COLOR "GDK ERROR:" GDK_DEFAULT_COLOR
                     " Failed setting CollideSprite hitbox "
                     "width/height - values may not be less than 0\n");
    return;
  }

  hitbox_xoffset = x_offset;
  hitbox_yoffset = y_offset;
  hitbox.x = x + x_offset;
  hitbox.y = y + y_offset;
  hitbox.w = width;
  hitbox.h = height;
}

SDL_Rect *CollideSprite::getHitbox() { return &hitbox; }

const bool CollideSprite::collided(CollideSprite *collide_sprite) {
  // Print warning if the function is checking whether the sprite collides
  // with itself.
  if (collide_sprite == this) {
    printf(GDK_WARNING_COLOR "GDK WARNING:" GDK_DEFAULT_COLOR
                      " Checking if GDK_CollideSprite collided with itself "
                      "- returning true\n");
    return true;
  }

  int collider_hbx = collide_sprite->getHitbox()->x;
  int collider_hby = collide_sprite->getHitbox()->y;
  int collider_hbw = collide_sprite->getHitbox()->w;
  int collider_hbh = collide_sprite->getHitbox()->h;

  if (collider_hbw == 0 || collider_hbh == 0) {
    printf(GDK_WARNING_COLOR "GDK WARNING:" GDK_DEFAULT_COLOR
                      " Checking collisions with hitbox which has width/height "
                      "values equal to 0. Are you sure the "
                      "hitbox is correctly initialized?\n");
  }

  bool collides = hitbox.x + hitbox.w >= collider_hbx &&
                  hitbox.x <= collider_hbx + collider_hbw &&
                  hitbox.y + hitbox.h >= collider_hby &&
                  hitbox.y <= collider_hby + collider_hbh;

  if (collides) {
    collided_sprites.push_back(collide_sprite);
    return true;
  } else {
    std::vector<gdk::CollideSprite *,
                std::allocator<gdk::CollideSprite *>>::iterator
        vector_position = std::find(collided_sprites.begin(),
                                    collided_sprites.end(), collide_sprite);

    if (vector_position != collided_sprites.end()) {
      collided_sprites.erase(vector_position);
    }

    return false;
  }
}

const bool CollideSprite::collided() {
  collided_sprites.clear();

  for (int i = 0; i < colliders->size(); i++) {
    if (colliders->at(i) != this && collided(colliders->at(i))) {
      continue;
    }
  }

  if (!collided_sprites.empty()) {
    return true;
  }

  return false;
}

std::vector<CollideSprite *> *CollideSprite::getCollidedSprites() {
  return &collided_sprites;
}
} // namespace gdk