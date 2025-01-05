#include <scene.hpp>

namespace gdk {
Scene::Scene() noexcept {}

Scene::Scene(SDL_Renderer *renderer, Camera *camera) noexcept {
  this->renderer = renderer;
  this->camera = camera;
}

void Scene::drawHitboxOutlines() {
  for (int i = 0; i < collide_sprites.size(); i++) {
    SDL_RenderDrawRect(renderer, collide_sprites.at(i)->getHitbox());
  }
}

void Scene::updateCamera() {
  // Print warning if camera is not set
  if (camera == nullptr) {
    printf(WARN_COLOR "GDK WARNING:" DEF_COLOR " Scene camera is not set\n");
    return;
  }

  // Move camera for all basic sprites
  for (int i = 0; i < basic_sprites.size(); i++) {
    // Make sure that sprite isn't a focused sprite,
    // and that camera has been moved
    if (std::find(camera->getFocusedSprites()->begin(),
                  camera->getFocusedSprites()->end(),
                  basic_sprites.at(i)) == camera->getFocusedSprites()->end() &&
        (camera->offset_x != 0 || camera->offset_y != 0)) {

      basic_sprites.at(i)->move(camera->offset_x, 0);
      basic_sprites.at(i)->move(0, camera->offset_y);
    }
  }

  // Move camera for all collide sprites
  for (int i = 0; i < collide_sprites.size(); i++) {
    // Make sure that collide sprite isn't a focused sprite,
    // and that camera has been moved
    if (std::find(camera->getFocusedSprites()->begin(),
                  camera->getFocusedSprites()->end(), collide_sprites.at(i)) ==
            camera->getFocusedSprites()->end() &&
        (camera->offset_x != 0 || camera->offset_y != 0)) {

      // We need to move collide sprite without the use of move() function in
      // this case, in order to avoid sprites colliding during camera movement
      collide_sprites.at(i)->x += camera->offset_x;
      collide_sprites.at(i)->y += camera->offset_y;
      collide_sprites.at(i)->updateHitboxCoords();
    }
  }

  camera->offset_x = 0;
  camera->offset_y = 0;
}

void Scene::setCamera(Camera *camera) noexcept {
  this->camera = camera;
}

void Scene::drawSprites() {
  for (int i = 0; i < sprites.size(); i++) {
    // Print error if scene renderer and sprite renderer do not match
    if (sprites.at(i)->texture != nullptr &&
        sprites.at(i)->texture->renderer != renderer) {
      printf(ERR_COLOR "GDK ERROR:" DEF_COLOR
                       " Failed drawing scene, sprite renderer and scene "
                       "renderer do not match\n");
      SDL_RenderClear(renderer);
      return;
    }

    if (sprites.at(i)->isInBounds()) {
      sprites.at(i)->draw();
    }
  }
}

void Scene::destroy() {
  basic_sprites.clear();
  collide_sprites.clear();
  sprites.clear();
}

void Scene::addCollideSprite(CollideSprite *collide_sprite) {
  sprites.push_back(collide_sprite);
  collide_sprite->setColliders(&collide_sprites);
  collide_sprites.push_back(collide_sprite);
}

void Scene::addCollideSprite(
    const std::vector<CollideSprite *> collide_sprites) {
  for (int i = 0; i < collide_sprites.size(); i++) {
    addCollideSprite(collide_sprites.at(i));
  }
}

void Scene::addSprite(Sprite *sprite) {
  sprites.push_back(sprite);
  basic_sprites.push_back(sprite);
}

void Scene::addSprite(const std::vector<Sprite *> sprites) {
  for (int i = 0; i < sprites.size(); i++) {
    addSprite(sprites.at(i));
  }
}

void Scene::removeCollideSprite(CollideSprite *collide_sprite) {
  collide_sprites.erase(std::remove(collide_sprites.begin(),
                                    collide_sprites.end(), collide_sprite),
                        collide_sprites.end());

  sprites.erase(std::remove(sprites.begin(), sprites.end(), collide_sprite),
                sprites.end());
}

void Scene::removeCollideSprite(
    const std::vector<CollideSprite *> collide_sprites) {
  for (int i = 0; i < collide_sprites.size(); i++) {
    removeCollideSprite(collide_sprites.at(i));
  }
}

void Scene::removeSprite(Sprite *sprite) {
  basic_sprites.erase(
      std::remove(basic_sprites.begin(), basic_sprites.end(), sprite),
      basic_sprites.end());

  sprites.erase(std::remove(sprites.begin(), sprites.end(), sprite),
                sprites.end());
}

void Scene::removeSprite(const std::vector<Sprite *> sprites) {
  for (int i = 0; i < sprites.size(); i++) {
    removeSprite(sprites.at(i));
  }
}

void Scene::draw() {
  SDL_RenderClear(renderer);
  SDL_GetRendererOutputSize(renderer, &win_w, &win_h);

  updateCamera();
  drawSprites();

  if (show_hitbox_outlines) {
    drawHitboxOutlines();
  }

  SDL_RenderPresent(renderer);
}

std::vector<CollideSprite *> *Scene::getAllCollideSprites() noexcept {
  return &collide_sprites;
}

std::vector<Sprite *> *Scene::getAllSprites() noexcept {
  return &sprites;
}

void Scene::setShowHitboxOutlines(bool show_hitbox_outlines) {
  SDL_SetRenderDrawColor(renderer, hitbox_outline_color.r,
                         hitbox_outline_color.g, hitbox_outline_color.b,
                         hitbox_outline_color.a);
  this->show_hitbox_outlines = show_hitbox_outlines;
}

void Scene::setHitboxOutlineColor(SDL_Color color) {
  hitbox_outline_color = color;
  SDL_SetRenderDrawColor(renderer, hitbox_outline_color.r,
                         hitbox_outline_color.g, hitbox_outline_color.b,
                         hitbox_outline_color.a);
}
} // namespace gdk