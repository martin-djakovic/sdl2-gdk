#ifndef CAMERA_HPP
#define CAMERA_HPP

#ifdef __linux__
#include <sdl2-gdk/collidesprite.hpp>
#include <sdl2-gdk/scene.hpp>
#include <sdl2-gdk/sprite.hpp>
#include <sdl2-gdk/apiprefix.h>

#elif _WIN32
#include <collidesprite.hpp>
#include <scene.hpp>
#include <sprite.hpp>
#include <apiprefix.h>

#elif __APPLE__
#include <collidesprite.hpp>
#include <scene.hpp>
#include <sprite.hpp>
#include <apiprefix.h>

#else
#error "FATAL ERROR: Operating system not supported"

#endif

#include <algorithm>
#include <vector>

namespace gdk {
class API_PREFIX Camera {

private:
  std::vector<Sprite *> focused_sprites;
  Scene *scene = nullptr;

public:
  Camera();

  /**
   * @param focused_sprites Sprites that will not get moved when moving camera
   */
  Camera(const std::vector<Sprite *> focused_sprites);

  Camera(Camera *camera);

  Camera(Camera *camera, Scene *scene);

  /**
   * @brief Attach scene to camera. This scene will be subject to the camera
   * movement
   */
  void attachScene(Scene *scene) noexcept;

  /**
   * @brief Detach the scene from camera
   */
  void detachScene() noexcept;

  /**
   * @brief Moves all unfocused sprites in the scene that has this camera
   */
  void move(double x, double y);

  /**
   * @brief Make a sprite focused. Focused sprites do not get moved when moving
   * camera
   */
  void addFocusedSprite(Sprite *sprite);

  /**
   * @brief Make multiple sprites focused. Focused sprites do not get moved
   * when moving camera
   */
  void addFocusedSprite(const std::vector<Sprite *> sprites);

  /**
   * @brief Unfocus sprite. Focused sprites do not get moved
   * when moving camera
   */
  void removeFocusedSprite(Sprite *sprite);

  /**
   * @brief Unfocus multiple sprites. Focused sprites do not get moved
   * when moving camera
   */
  void removeFocusedSprite(const std::vector<Sprite *> sprites);

  /**
   * @brief Sets focused sprites. Focused sprites do not get moved
   * when moving camera
   */
  void setFocusedSprites(const std::vector<Sprite *> sprites);

  /**
   * @brief Gets focused sprites. Focused sprites do not get moved
   * when moving camera
   */
  std::vector<Sprite *> *getFocusedSprites() noexcept;
};
} // namespace gdk

#endif
