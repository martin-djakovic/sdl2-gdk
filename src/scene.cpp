#include <scene.hpp>

GDK_Scene::GDK_Scene() noexcept {}

GDK_Scene::GDK_Scene(SDL_Renderer *renderer, GDK_Camera *camera) noexcept {
  this->renderer = renderer;
  this->camera = camera;
}

void GDK_Scene::calculateFPS() {
  if (odd_frame) {
    last_frame_time = SDL_GetPerformanceCounter();
    odd_frame = !odd_frame;
  } else {
    Uint64 this_frame_time = SDL_GetPerformanceCounter();
    fps = 1.0f / ((this_frame_time - last_frame_time) /
                  (float)SDL_GetPerformanceFrequency());
    odd_frame = !odd_frame;
  }
}

void GDK_Scene::drawHitboxOutlines() {
  for (int i = 0; i < collide_sprites.size(); i++) {
    SDL_RenderDrawRect(renderer, collide_sprites.at(i)->getHitbox());
  }
}

void GDK_Scene::updateCamera() {
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

void GDK_Scene::setCamera(GDK_Camera *camera) noexcept {
  this->camera = camera;
}

void GDK_Scene::drawSprites() {
  for (int i = 0; i < sprites.size(); i++) {
    // Print error if scene renderer and sprite renderer do not match
    if (sprites.at(i)->renderer != renderer) {
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

void GDK_Scene::destroy() {
  basic_sprites.clear();
  collide_sprites.clear();
  sprites.clear();
}

void GDK_Scene::addCollideSprite(GDK_CollideSprite *collide_sprite) {
  sprites.push_back(collide_sprite);
  collide_sprite->setColliders(&collide_sprites);
  collide_sprites.push_back(collide_sprite);
}

void GDK_Scene::addCollideSprite(
    const std::vector<GDK_CollideSprite *> collide_sprites) {
  for (int i = 0; i < collide_sprites.size(); i++) {
    addCollideSprite(collide_sprites.at(i));
  }
}

void GDK_Scene::addSprite(GDK_Sprite *sprite) {
  sprites.push_back(sprite);
  basic_sprites.push_back(sprite);
}

void GDK_Scene::addSprite(const std::vector<GDK_Sprite *> sprites) {
  for (int i = 0; i < sprites.size(); i++) {
    addSprite(sprites.at(i));
  }
}

void GDK_Scene::removeCollideSprite(GDK_CollideSprite *collide_sprite) {
  collide_sprites.erase(std::remove(collide_sprites.begin(),
                                    collide_sprites.end(), collide_sprite),
                        collide_sprites.end());

  sprites.erase(std::remove(sprites.begin(), sprites.end(), collide_sprite),
                sprites.end());
}

void GDK_Scene::removeCollideSprite(
    const std::vector<GDK_CollideSprite *> collide_sprites) {
  for (int i = 0; i < collide_sprites.size(); i++) {
    removeCollideSprite(collide_sprites.at(i));
  }
}

void GDK_Scene::removeSprite(GDK_Sprite *sprite) {
  basic_sprites.erase(
      std::remove(basic_sprites.begin(), basic_sprites.end(), sprite),
      basic_sprites.end());

  sprites.erase(std::remove(sprites.begin(), sprites.end(), sprite),
                sprites.end());
}

void GDK_Scene::removeSprite(const std::vector<GDK_Sprite *> sprites) {
  for (int i = 0; i < sprites.size(); i++) {
    removeSprite(sprites.at(i));
  }
}

void GDK_Scene::draw() {
  calculateFPS();

  SDL_RenderClear(renderer);
  SDL_GetRendererOutputSize(renderer, &win_w, &win_h);

  updateCamera();
  drawSprites();

  if (show_hitbox_outlines) {
    drawHitboxOutlines();
  }

  SDL_RenderPresent(renderer);
}

std::vector<GDK_CollideSprite *> *GDK_Scene::getAllCollideSprites() noexcept {
  return &collide_sprites;
}

std::vector<GDK_Sprite *> *GDK_Scene::getAllSprites() noexcept {
  return &sprites;
}

const int GDK_Scene::getFPS() { return fps; }

void GDK_Scene::setShowHitboxOutlines(bool show_hitbox_outlines) {
  SDL_SetRenderDrawColor(renderer, hitbox_outline_color.r,
                         hitbox_outline_color.g, hitbox_outline_color.b,
                         hitbox_outline_color.a);
  this->show_hitbox_outlines = show_hitbox_outlines;
}

void GDK_Scene::setHitboxOutlineColor(SDL_Color color) {
  hitbox_outline_color = color;
  SDL_SetRenderDrawColor(renderer, hitbox_outline_color.r,
                         hitbox_outline_color.g, hitbox_outline_color.b,
                         hitbox_outline_color.a);
}

const double GDK_Scene::getPerformanceMultiplier() noexcept {
  // Check if at least 2 frames have been drawn
  if (fps == -1) {
    printf(WARN_COLOR "GDK WARNING:" DEF_COLOR
                      " Not enough frame samples to calculate performance "
                      "multiplier, returning 1\n");
    return 1;
  }

  // Print error if FPS is too low (gets rounded to 0)
  if (fps == 0) {
    printf(WARN_COLOR
           "GDK WARNING:" DEF_COLOR
           " FPS is too low to get performance multiplier, returning 1\n");
    return 1;
  }

  return 100.0f / fps;
}
