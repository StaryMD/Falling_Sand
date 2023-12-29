#ifndef CAMERA_VIEW_HPP_
#define CAMERA_VIEW_HPP_

#include <array>
#include <cstdint>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

class CameraView {
 public:
  CameraView() = delete;

  CameraView(sf::Vector2<uint32_t> max_size, sf::Vector2<uint32_t> size, sf::Vector2<uint32_t> pos_center);

  [[nodiscard]] double GetZoomLevel() const;

  void Zoom(float delta, sf::Vector2<int32_t> location);

  void MovePosition(sf::Vector2<int32_t> delta);

  [[nodiscard]] sf::Vector2<double> MapPixelToCoords(sf::Vector2<int32_t> position) const;

  [[nodiscard]] sf::Vector2<int32_t> MapCoordsToPixel(sf::Vector2<double> coords) const;

  [[nodiscard]] sf::Rect<double> GetFieldOfView() const;

 private:
  sf::Vector2<int32_t> max_size_;
  sf::Rect<double> field_of_view_;
  int32_t zoom_level_;

  void ClampViewToMaxSize();

  static constexpr int32_t kZoomLevelCount = 6;
  static constexpr int32_t kStartingZoomLevel = 2;
  static constexpr std::array<double, kZoomLevelCount> kZoomLevels{1.0, 2.0, 4.0, 8.0, 16.0, 32.0};
};

#endif /* CAMERA_VIEW_HPP_ */
