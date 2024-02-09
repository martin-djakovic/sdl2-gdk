#include <collidesprite.hpp>

CollideSprite::CollideSprite(double x, double y, int width, int height,
                             const char *img_path, double speed,
                             bool auto_set_size, double rotation_angle,
                             SDL_RendererFlip flip)
    : BasicSprite(x, y, width, height, img_path, speed, auto_set_size,
                  rotation_angle, flip) {
    SetHitboxParams(0, 0, w, h);
}

bool CollideSprite::MovementCollided(CollideSprite *collide_sprite) {
    int collider_hbx = collide_sprite->GetHitbox()->x;
    int collider_hby = collide_sprite->GetHitbox()->y;
    int collider_hbw = collide_sprite->GetHitbox()->w;
    int collider_hbh = collide_sprite->GetHitbox()->h;

    if (hitbox.x + hitbox.w > collider_hbx &&
        hitbox.x < collider_hbx + collider_hbw &&
        hitbox.y + hitbox.h > collider_hby &&
        hitbox.y < collider_hby + collider_hbh) {

        this->grad_mvmt_iter = 0;
        return true;
    } else {
        return false;
    }
}

void CollideSprite::UpdateHitboxCoords() {
    hitbox.x = x + hitbox_xoffset;
    hitbox.y = y + hitbox_yoffset;
}

void CollideSprite::SetX(double x, bool check_collision) {
    this->x = x;
    UpdateHitboxCoords();

    if (check_collision) {
        for (int i = 0; i < colliders->size(); i++) {
            double collider_x = colliders->at(i)->GetHitbox()->x;
            double collider_w = colliders->at(i)->GetHitbox()->w;

            if (colliders->at(i) != this &&
                MovementCollided(colliders->at(i))) {

                if (collider_x > hitbox.x) {
                    this->x = collider_x - hitbox.w - hitbox_xoffset;
                    UpdateHitboxCoords();
                } else {
                    this->x = collider_x + collider_w - hitbox_xoffset;
                    UpdateHitboxCoords();
                }

                return;
            }
        }
    }
}

void CollideSprite::SetY(double y, bool check_collision) {
    this->y = y;
    UpdateHitboxCoords();

    if (check_collision) {
        for (int i = 0; i < colliders->size(); i++) {
            double collider_y = colliders->at(i)->GetHitbox()->y;
            double collider_h = colliders->at(i)->GetHitbox()->h;

            if (colliders->at(i) != this &&
                MovementCollided(colliders->at(i))) {

                if (collider_y > hitbox.y) {
                    this->y = collider_y - hitbox.h - hitbox_yoffset;
                    UpdateHitboxCoords();
                } else {
                    this->y = collider_y + collider_h - hitbox_yoffset;
                    UpdateHitboxCoords();
                }

                return;
            }
        }
    }
}

void CollideSprite::MoveX(double x) { SetX(this->x + x, true); }

void CollideSprite::MoveX(double x, bool check_collision) {
    SetX(this->x + x, check_collision);
}

void CollideSprite::MoveX(GDK_Direction direction) {
    MoveX(speed * ConvertDirectionToInt(direction), true);
}

void CollideSprite::MoveX(GDK_Direction direction, bool check_collision) {
    MoveX(speed * ConvertDirectionToInt(direction), check_collision);
}

void CollideSprite::MoveY(double y) { SetY(this->y + y, true); }

void CollideSprite::MoveY(double y, bool check_collision) {
    SetY(this->y + y, check_collision);
}

void CollideSprite::MoveY(GDK_Direction direction) {
    MoveY(speed * ConvertDirectionToInt(direction), true);
}

void CollideSprite::MoveY(GDK_Direction direction, bool check_collision) {
    MoveY(speed * ConvertDirectionToInt(direction), check_collision);
}

void CollideSprite::SetColliders(std::vector<CollideSprite *> *colliders) {
    this->colliders = colliders;
}

void CollideSprite::SetHitboxParams(double x_offset, double y_offset, int width,
                                    int height) {
    hitbox_xoffset = x_offset;
    hitbox_yoffset = y_offset;
    hitbox.x = x + x_offset;
    hitbox.y = y + y_offset;
    hitbox.w = width;
    hitbox.h = height;
}

SDL_Rect *CollideSprite::GetHitbox() { return &hitbox; }

bool CollideSprite::Collided(CollideSprite *collide_sprite) {
    int collider_hbx = collide_sprite->GetHitbox()->x;
    int collider_hby = collide_sprite->GetHitbox()->y;
    int collider_hbw = collide_sprite->GetHitbox()->w;
    int collider_hbh = collide_sprite->GetHitbox()->h;

    if (hitbox.x + hitbox.w >= collider_hbx &&
        hitbox.x <= collider_hbx + collider_hbw &&
        hitbox.y + hitbox.h >= collider_hby &&
        hitbox.y <= collider_hby + collider_hbh && collide_sprite != this) {

        this->collide_sprite = collide_sprite;
        return true;
    } else {
        return false;
    }
}

bool CollideSprite::Collided() {
    for (int i = 0; i < colliders->size(); i++) {

        if (Collided(colliders->at(i))) {
            return true;
        }
    }

    return false;
}

CollideSprite *CollideSprite::GetCollideSprite() { return collide_sprite; }