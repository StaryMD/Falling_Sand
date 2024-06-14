#ifndef CAMERA_VIEW_HPP_
#define CAMERA_VIEW_HPP_

#include <array>
#include <cstdint>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

class CameraView {
 public:
  CameraView() = delete;

  /**
   * @brief Construct CameraView.
   * 
   * @param max_size Maximum size of the FOV.
   * @param size Starting size of the FOV.
   * @param pos_center Center of the FOV.
   */
  CameraView(sf::Vector2<uint32_t> max_size, sf::Vector2<uint32_t> size,
             sf::Vector2<uint32_t> pos_center);

  /**
   * @brief Get the current zoom level.
   * 
   * @return double The zoom level.
   */
  [[nodiscard]] double GetZoomLevel() const;

  /**
   * @brief Change the size of the FOV relative to a location.
   * 
   * @param delta How much the FOV should be changed - in zoom levels.
   * @param location Location of the center of the zoom.
   */
  void Zoom(float delta, sf::Vector2<int32_t> location);

  /**
   * @brief Move the FOV relative to the current location.
   * 
   * @param delta Coordinates to be added to the current location of the FOV.
   */
  void MovePosition(sf::Vector2<int32_t> delta);

  /**
   * @brief Map a pair of coordinates from screen-space to map-space.
   * 
   * @param coords Coordinates to be mapped.
   * @return sf::Vector2<int32_t> Map-space coordinates.
   */
  [[nodiscard]] sf::Vector2<double> MapPixelToCoords(sf::Vector2<int32_t> position) const;

  /**
   * @brief Map a pair of coordinates from map-space to screen-space.
   * 
   * @param position Position to be mapped.
   * @return sf::Vector2<double> Screen-space coordinates of the position.
   */
  [[nodiscard]] sf::Vector2<int32_t> MapCoordsToPixel(sf::Vector2<double> coords) const;

  /**
   * @brief Get the current FOV.
   * 
   * @return sf::Rect<double> Rectangle containing the top-left position and the size of the FOV.
   */
  [[nodiscard]] sf::Rect<double> GetView() const;

 private:
  sf::Vector2<int32_t> max_size_;
  sf::Rect<double> view_;
  int32_t zoom_level_;

  /**
   * @brief Clamp the FOV to **max_size_**.
   * 
   */
  void ClampViewToMaxSize();

  static constexpr int32_t kZoomLevelCount = 6;
  static constexpr int32_t kStartingZoomLevel = 2;
  static constexpr std::array<double, kZoomLevelCount> kZoomLevels{1.0, 2.0, 4.0, 8.0, 16.0, 32.0};
};

#endif  // CAMERA_VIEW_HPP_
