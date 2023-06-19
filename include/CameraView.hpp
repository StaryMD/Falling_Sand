#ifndef CAMERA_VIEW_HPP_
#define CAMERA_VIEW_HPP_

#include <SFML/Graphics/Rect.hpp>
#include <array>

#include <SFML/System/Vector2.hpp>

class CameraView {
 public:
  CameraView() = delete;

  CameraView(sf::Vector2i max_size, sf::Vector2i size, sf::Vector2i pos_center);

  CameraView(sf::Vector2u max_size, sf::Vector2u size, sf::Vector2u pos_center);

  void Zoom(float delta, sf::Vector2i location);

  void MovePosition(sf::Vector2i delta);

  [[nodiscard]] double GetZoomLevel() const;

  [[nodiscard]] sf::Rect<double> GetFieldOfView() const;

 private:
  sf::Vector2i max_size_;
  sf::Rect<double> field_of_view_;
  int zoom_level_;

  static constexpr int kZoomLevelCount = 7;
  static constexpr std::array<double, kZoomLevelCount> kZoomLevels{1.0, 2.0, 3.0, 4.0, 8.0, 16.0, 32.0};
};

#endif /* CAMERA_VIEW_HPP_ */
