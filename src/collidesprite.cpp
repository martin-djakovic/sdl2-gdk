#include <collidesprite.hpp>
#include <cstdio>
#include <texture.hpp>

GDK_CollideSprite::GDK_CollideSprite() : GDK_Sprite() {
  setHitboxProperties(0, 0, 0, 0);
}

GDK_CollideSprite::GDK_CollideSprite(GDK_ImageTexture *texture)
    : GDK_Sprite(texture) {
  setHitboxProperties(0, 0, 0, 0);
}

GDK_CollideSprite::GDK_CollideSprite(GDK_ImageTexture *texture, double x, double y,
                                     int width, int height)
    : GDK_Sprite(texture, x, y, width, height) {
  setHitboxProperties(0, 0, width, height);
}

const bool
GDK_CollideSprite::movementCollided(GDK_CollideSprite *collide_sprite) {
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
    this->grad_mvmt_iter = 0;
    return true;
  } else {
    return false;
  }
}

void GDK_CollideSprite::updateHitboxCoords() noexcept {
  hitbox.x = x + hitbox_xoffset;
  hitbox.y = y + hitbox_yoffset;
}

void GDK_CollideSprite::setEnableMovementCollision(
    bool enable_movement_collision) noexcept {
  this->enable_movement_collision = enable_movement_collision;
}

const bool GDK_CollideSprite::getEnableMovementCollision() noexcept {
  return enable_movement_collision;
}

void GDK_CollideSprite::setPosition(double x, double y) noexcept {
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

void GDK_CollideSprite::move(double x, double y) noexcept {
  setPosition(this->x + x, this->y + y);
}

void GDK_CollideSprite::setColliders(
    std::vector<GDK_CollideSprite *> *colliders) {
  this->colliders = colliders;
}

void GDK_CollideSprite::setHitboxProperties(double x_offset, double y_offset,
                                            int width, int height) {
  // Print warning if hitbox width/height is 0, because then collisions can't
  // be checked
  if (width == 0 || height == 0) {
    printf(WARN_COLOR "GDK WARNING:" DEF_COLOR
                      " Hitbox width/height of GDK_CollideSprite set to 0 "
                      "- will be unable to perform collisions");
  }

  // Print error if hitbox width/height is set to less than 0
  if (width < 0 || height < 0) {
    printf(ERR_COLOR "GDK ERROR:" DEF_COLOR
                     " Failed setting GDK_CollideSprite hitbox "
                     "width/height - values may not be less than 0");
    return;
  }

  hitbox_xoffset = x_offset;
  hitbox_yoffset = y_offset;
  hitbox.x = x + x_offset;
  hitbox.y = y + y_offset;
  hitbox.w = width;
  hitbox.h = height;
}

SDL_Rect *GDK_CollideSprite::getHitbox() { return &hitbox; }

const bool GDK_CollideSprite::collided(GDK_CollideSprite *collide_sprite) {
  // Print warning if the function is checking whether the sprite collides
  // with itself.
  if (collide_sprite == this) {
    printf(WARN_COLOR "GDK WARNING:" DEF_COLOR
                      " Checking if GDK_CollideSprite collided with itself "
                      "- returning true");
    return true;
  }

  int collider_hbx = collide_sprite->getHitbox()->x;
  int collider_hby = collide_sprite->getHitbox()->y;
  int collider_hbw = collide_sprite->getHitbox()->w;
  int collider_hbh = collide_sprite->getHitbox()->h;

  if (collider_hbw == 0 || collider_hbh == 0) {
    printf(WARN_COLOR "GDK WARNING:" DEF_COLOR
                      " Checking collisions with hitbox which has width/height "
                      "values equal to 0. Are you sure the "
                      "hitbox is correctly initialized?");
  }

  bool collides = hitbox.x + hitbox.w >= collider_hbx &&
                  hitbox.x <= collider_hbx + collider_hbw &&
                  hitbox.y + hitbox.h >= collider_hby &&
                  hitbox.y <= collider_hby + collider_hbh;

  if (collides) {
    this->collide_sprite = collide_sprite;
    return true;
  } else {
    return false;
  }
}

const bool GDK_CollideSprite::collided() {
  for (int i = 0; i < colliders->size(); i++) {
    if (colliders->at(i) != this && collided(colliders->at(i))) {
      return true;
    }
  }

  return false;
}

GDK_CollideSprite *GDK_CollideSprite::getCollideSprite() {
  return collide_sprite;
}
