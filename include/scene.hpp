#ifndef SCENE_HPP
#define SCENE_HPP

#include <algorithm>
#include <sdl2-gdk/collidesprite.hpp>
#include <sdl2-gdk/errorcolors.hpp>
#include <vector>

namespace gdk {
class Scene {
private:
  std::vector<CollideSprite *> collide_sprites;
  std::vector<Sprite *> basic_sprites;
  // Every sprite that should be drawn in the scene
  std::vector<Sprite *> sprites;

  int win_w;
  int win_h;

  bool show_hitbox_outlines = false;
  SDL_Color hitbox_outline_color = {0, 255, 0};

  SDL_Renderer *renderer = nullptr;

  /**
   * @brief Draws hitbox outlines of all collide sprites
   */
  void drawHitboxOutlines();

  /**
   * @brief Draws all sprites in the scene
   */
  void drawSprites();

public:
  Scene() noexcept;

  /**
   * @param camera Camera object for this scene. Camera movement is updated when
   * scene is drawn
   * @param renderer SDL_Renderer that scene will be drawn on. All textures in
   * the scene must have the same renderer as the scene
   */
  Scene(SDL_Renderer *renderer) noexcept;

  Scene(Scene *scene);

  /**
   * @brief Frees scene from memory. Doesn't destroy scene sprites
   */
  void destroy();

  /**
   * @brief Adds collide sprite to scene. Sets colliders of collide sprite to
   * all collide sprites in the scene
   */
  void addCollideSprite(CollideSprite *collide_sprite);

  /**
   * @brief Adds collide sprites to scene. Sets colliders of collide sprites to
   * all collide sprites in the scene
   * @param sprites Vector of collide sprites to be added
   */
  void addCollideSprite(const std::vector<CollideSprite *> collide_sprites);

  /**
   * @brief Adds sprite to scene
   * @param sprites Sprite to be added
   */
  void addSprite(Sprite *sprite);

  /**
   * @brief Adds sprites to scene
   * @param sprites Vector of sprites to be added
   */
  void addSprite(const std::vector<Sprite *> sprites);

  /**
   * @brief Removes collide sprite from scene
   * @param sprites Collide sprite to be removed
   */
  void removeCollideSprite(CollideSprite *collide_sprite);

  /**
   * @brief Removes collide sprites from scene
   * @param sprites Vector containing collide sprites to be removed
   */
  void removeCollideSprite(const std::vector<CollideSprite *> collide_sprites);

  /**
   * @brief Removes sprite from scene
   * @param sprite Sprite to be removed
   */
  void removeSprite(Sprite *sprite);

  /**
   * @brief Removes sprites from scene
   * @param sprites Vector containing sprites to be removed
   */
  void removeSprite(const std::vector<Sprite *> sprites);

  /**
   * @brief Updates drawing order based on sprite z-indices. Sprites with lowest
   * z-index are drawn first
   */
  void updateDrawOrder();

  /**
   * @brief Draws all visible sprites in the scene. Applies camera movement
   */
  void draw();

  /**
   * @return All collide sprites in the scene
   */
  std::vector<CollideSprite *> *getCollideSprites() noexcept;

  /**
   * @return All non-collide sprites in the scene
   */
  std::vector<Sprite *> *getBasicSprites() noexcept;

  /**
   * @return All sprites in the scene
   */
  std::vector<Sprite *> *getAllSprites() noexcept;

  /**
   * @brief Determine if collide sprite hitbox outlines should be drawn.
   * @param show_hitbox_outlines If true, hitbox outlines are drawn, if false
   * they are not drawn
   */
  void setShowHitboxOutlines(bool show_hitbox_outlines);

  /**
   * @brief Set color of collide sprite hitbox outlines
   */
  void setHitboxOutlineColor(SDL_Color color);
};
} // namespace gdk

#endif