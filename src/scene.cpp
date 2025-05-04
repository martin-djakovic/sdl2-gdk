#include <sdl2-gdk/scene.hpp>

namespace gdk {
Scene::Scene() noexcept {}

Scene::Scene(SDL_Renderer *renderer) noexcept { this->renderer = renderer; }

Scene::Scene(Scene *scene) {
  renderer = scene->renderer;
  addCollideSprite(*scene->getCollideSprites());
  addSprite(*scene->getBasicSprites());
  setShowHitboxOutlines(scene->show_hitbox_outlines);
  setHitboxOutlineColor(scene->hitbox_outline_color);
}

void Scene::drawHitboxOutlines() {
  SDL_SetRenderDrawColor(renderer, hitbox_outline_color.r,
                         hitbox_outline_color.g, hitbox_outline_color.b,
                         hitbox_outline_color.a);

  for (int i = 0; i < collide_sprites.size(); i++) {
    SDL_RenderDrawRect(renderer, collide_sprites.at(i)->getHitbox());
  }

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
}

void Scene::drawSprites() {
  for (int i = 0; i < sprites.size(); i++) {
    // Print error if scene renderer and sprite renderer do not match
    if (sprites.at(i)->texture != nullptr &&
        sprites.at(i)->texture->renderer != renderer) {
      printf(GDK_ERROR_COLOR "GDK ERROR:" GDK_DEFAULT_COLOR
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
  updateDrawOrder();
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
  updateDrawOrder();
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

  updateDrawOrder();
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

  updateDrawOrder();
}

void Scene::removeSprite(const std::vector<Sprite *> sprites) {
  for (int i = 0; i < sprites.size(); i++) {
    removeSprite(sprites.at(i));
  }
}

void Scene::updateDrawOrder() {
  std::sort(sprites.begin(), sprites.end(), Sprite::comparePtr);
}

void Scene::draw() {
  SDL_RenderClear(renderer);
  SDL_GetRendererOutputSize(renderer, &win_w, &win_h);

  drawSprites();

  if (show_hitbox_outlines) {
    drawHitboxOutlines();
  }

  SDL_RenderPresent(renderer);
}

std::vector<CollideSprite *> *Scene::getCollideSprites() noexcept {
  return &collide_sprites;
}

std::vector<Sprite *> *Scene::getBasicSprites() noexcept {
  return &basic_sprites;
}

std::vector<Sprite *> *Scene::getAllSprites() noexcept { return &sprites; }

void Scene::setShowHitboxOutlines(bool show_hitbox_outlines) {
  SDL_SetRenderDrawColor(renderer, hitbox_outline_color.r,
                         hitbox_outline_color.g, hitbox_outline_color.b,
                         hitbox_outline_color.a);
  this->show_hitbox_outlines = show_hitbox_outlines;
}

void Scene::setHitboxOutlineColor(SDL_Color color) {
  hitbox_outline_color = color;
}
} // namespace gdk