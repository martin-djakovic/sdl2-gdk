#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <algorithm>
#include <basicsprite.hpp>
#include <vector>

class GDK_Camera {
  friend class GDK_Scene;

private:
  std::vector<GDK_Sprite *> focused_sprites;
  /* Values determining how much sprites in scene should be moved in the next
   * frame Changes when moving camera Gets set to 0 when frame is drawn
   */
  double offset_x = 0;
  double offset_y = 0;

public:
  GDK_Camera();

  /*
   * @param focused_sprites sprites that will not get moved when moving camera
   */
  GDK_Camera(const std::vector<GDK_Sprite *> focused_sprites);

  /*
   * @brief Moves all unfocused sprites in the scene that has this camera
   */
  void move(double x, double y) noexcept;

  /*
   * @brief Make a sprite focused. Focused sprites do not get moved when moving
   * camera
   */
  void addFocusedSprite(GDK_Sprite *sprite);

  /*
   * @brief Make multiple sprites focused. Focused sprites do not get moved
   * when moving camera
   */
  void addFocusedSprite(const std::vector<GDK_Sprite *> sprites);

  /*
   * @brief Unfocus sprite. Focused sprites do not get moved
   * when moving camera
   */
  void removeFocusedSprite(GDK_Sprite *sprite);

  /*
   * @brief Unfocus multiple sprites. Focused sprites do not get moved
   * when moving camera
   */
  void removeFocusedSprite(const std::vector<GDK_Sprite *> sprites);

  /*
   * @brief Sets focused sprites. Focused sprites do not get moved
   * when moving camera
   */
  void setFocusedSprites(const std::vector<GDK_Sprite *> sprites);

  /*
   * @brief Gets focused sprites. Focused sprites do not get moved
   * when moving camera
   */
  std::vector<GDK_Sprite *> *getFocusedSprites() noexcept;
};

#endif
