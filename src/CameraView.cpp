#include "CameraView.hpp"

#include <algorithm>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

CameraView::CameraView(const sf::Vector2i max_size, const sf::Vector2i size, const sf::Vector2i pos_center)
    : max_size_(max_size), zoom_level_(0) {
  const sf::Vector2<double> local_size = {static_cast<double>(size.x), static_cast<double>(size.y)};
  const sf::Vector2<double> local_pos_center = {static_cast<double>(pos_center.x), static_cast<double>(pos_center.y)};

  constexpr double kHalf = 0.5;
  field_of_view_ = sf::Rect<double>(local_pos_center - local_size * kHalf, local_size);
}

CameraView::CameraView(const sf::Vector2u max_size, const sf::Vector2u size, const sf::Vector2u pos_center)
    : max_size_(max_size), zoom_level_(0) {
  const sf::Vector2<double> local_size = {static_cast<double>(size.x), static_cast<double>(size.y)};
  const sf::Vector2<double> local_pos_center = {static_cast<double>(pos_center.x), static_cast<double>(pos_center.y)};

  constexpr double kHalf = 0.5;
  field_of_view_ = sf::Rect<double>(local_pos_center - local_size * kHalf, local_size);
}

void CameraView::Zoom(const float delta, const sf::Vector2i location) {
  const double prev_zoom_level = GetZoomLevel();
  zoom_level_ = std::clamp(zoom_level_ + static_cast<int>(delta), 0, kZoomLevelCount - 1);

  const double ratio = prev_zoom_level / GetZoomLevel();

  field_of_view_.width = field_of_view_.width * ratio;
  field_of_view_.height = field_of_view_.height * ratio;

  field_of_view_.left += (location.x / GetZoomLevel() - field_of_view_.left) * ratio;
  field_of_view_.top += (location.y / GetZoomLevel() - field_of_view_.top) * ratio;

  field_of_view_.left = std::clamp(field_of_view_.left, 0.0, static_cast<double>(max_size_.x) - field_of_view_.width);
  field_of_view_.top = std::clamp(field_of_view_.top, 0.0, static_cast<double>(max_size_.y) - field_of_view_.height);
}

void CameraView::MovePosition(const sf::Vector2i delta) {
  field_of_view_.left += delta.x / GetZoomLevel();
  field_of_view_.top += delta.y / GetZoomLevel();

  field_of_view_.left = std::clamp(field_of_view_.left, 0.0, static_cast<double>(max_size_.x) - field_of_view_.width);
  field_of_view_.top = std::clamp(field_of_view_.top, 0.0, static_cast<double>(max_size_.y) - field_of_view_.height);
}

double CameraView::GetZoomLevel() const {
  return kZoomLevels[zoom_level_];
}

sf::Rect<double> CameraView::GetFieldOfView() const {
  return field_of_view_;
}

sf::Vector2i ToVector2i(const sf::Vector2f vector) {
  return {static_cast<int>(vector.x), static_cast<int>(vector.y)};
}
