#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <algorithm>
#include <sprite.hpp>
#include <vector>

namespace gdk {
class Camera {
  friend class Scene;

private:
  std::vector<Sprite *> focused_sprites;
  /* Values determining how much sprites in scene should be moved in the next
   * frame Changes when moving camera Gets set to 0 when frame is drawn
   */
  double offset_x = 0;
  double offset_y = 0;

public:
  Camera();

  /**
   * @param focused_sprites sprites that will not get moved when moving camera
   */
  Camera(const std::vector<Sprite *> focused_sprites);

  /**
   * @brief Moves all unfocused sprites in the scene that has this camera
   */
  void move(double x, double y) noexcept;

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
