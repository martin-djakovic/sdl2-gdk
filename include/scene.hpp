#ifndef SCENE_HPP
#define SCENE_HPP

#include <camera.hpp>
#include <collidesprite.hpp>
#include <errorcolors.hpp>
#include <vector>

class GDK_Scene {
private:
  std::vector<GDK_CollideSprite *> collide_sprites;
  std::vector<GDK_Sprite *> basic_sprites;
  // Every sprite that should be drawn in the scene
  std::vector<GDK_Sprite *> sprites;

  int win_w;
  int win_h;

  bool show_hitbox_outlines = false;
  SDL_Color hitbox_outline_color = {0, 255, 0};

  GDK_Camera *camera = nullptr;

  SDL_Renderer *renderer = nullptr;

  /*
   * @brief Draws hitbox outlines of all collide sprites
   */
  void drawHitboxOutlines();

  /*
   * @brief Draws all sprites in the scene
   */
  void drawSprites();

  /*
   * @brief Completes camera movement
   */
  void updateCamera();

public:
  GDK_Scene() noexcept;

  /*
   * @param camera Camera object for this scene. Camera movement is updated when
   * scene is drawn
   * @param renderer SDL_Renderer that scene will be drawn on. All textures in
   * the scene must have the same renderer as the scene
   */
  GDK_Scene(SDL_Renderer *renderer, GDK_Camera *camera) noexcept;

  /*
   * @brief Set scene camera. Camera movement is updated when scene is drawn
   */
  void setCamera(GDK_Camera *camera) noexcept;

  /*
   * @brief Frees scene from memory. Doesn't destroy scene sprites
   */
  void destroy();

  /*
   * @brief Adds collide sprite to scene. Sets colliders of collide sprite to
   * all collide sprites in the scene
   */
  void addCollideSprite(GDK_CollideSprite *collide_sprite);

  /* @brief Adds collide sprites to scene. Sets colliders of collide sprites to
   * all collide sprites in the scene
   * @param sprites Vector of collide sprites to be added
   */
  void addCollideSprite(const std::vector<GDK_CollideSprite *> collide_sprites);

  /*
   * @brief Adds sprite to scene
   * @param sprites Sprite to be added
   */
  void addSprite(GDK_Sprite *sprite);

  /*
   * @brief Adds sprites to scene
   * @param sprites Vector of sprites to be added
   */
  void addSprite(const std::vector<GDK_Sprite *> sprites);

  /*
   * @brief Removes collide sprite from scene
   * @param sprites Collide sprite to be removed
   */
  void removeCollideSprite(GDK_CollideSprite *collide_sprite);

  /*
   * @brief Removes collide sprites from scene
   * @param sprites Vector containing collide sprites to be removed
   */
  void removeCollideSprite(const std::vector<GDK_CollideSprite *> collide_sprites);

  /*
   * @brief Removes sprite from scene
   * @param sprite Sprite to be removed
   */
  void removeSprite(GDK_Sprite *sprite);

  /*
   * @brief Removes sprites from scene
   * @param sprites Vector containing sprites to be removed
   */
  void removeSprite(const std::vector<GDK_Sprite *> sprites);

  /*
   * @brief Draws all visible sprites in the scene. Applies camera movement
   */
  void draw();

  /*
   * @return All collide sprites in the scene
   */
  std::vector<GDK_CollideSprite *> *getAllCollideSprites() noexcept;

  /*
   * @return All sprites in the scene
   */
  std::vector<GDK_Sprite *> *getAllSprites() noexcept;

  /*
   * @brief Determine if collide sprite hitbox outlines should be drawn.
   * @param show_hitbox_outlines If true, hitbox outlines are drawn, if false
   * they are not drawn
   */
  void setShowHitboxOutlines(bool show_hitbox_outlines);

  /*
   * @brief Set color of collide sprite hitbox outlines
   */
  void setHitboxOutlineColor(SDL_Color color);
};

#endif