#include "MasterEngine/CameraView.hpp"

#include <algorithm>
#include <cstdint>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include "CommonUtility.hpp"

CameraView::CameraView(const sf::Vector2<uint32_t> max_size, const sf::Vector2<uint32_t> size,
                       const sf::Vector2<uint32_t> pos_center)
    : max_size_(max_size), zoom_level_(0) {
  const sf::Vector2<double> local_size = {static_cast<double>(size.x), static_cast<double>(size.y)};
  const sf::Vector2<double> local_pos_center = {static_cast<double>(pos_center.x),
                                                static_cast<double>(pos_center.y)};

  constexpr double kHalf = 0.5;
  view_ = sf::Rect<double>(local_pos_center - local_size * kHalf, local_size);

  Zoom(kStartingZoomLevel, ToVector2<int32_t>(pos_center));
}

void CameraView::Zoom(float delta, const sf::Vector2<int32_t> location) {
  const sf::Vector2<double> mapped_location = MapPixelToCoords(location);

  const double prev_zoom_level = GetZoomLevel();
  if (delta > 0) {
    delta = 1.0;
  } else {
    delta = -1.0;
  }
  zoom_level_ = std::clamp(zoom_level_ + static_cast<int32_t>(delta), 0, kZoomLevelCount - 1);

  if (prev_zoom_level == GetZoomLevel()) {
    return;
  }

  const double ratio = prev_zoom_level / GetZoomLevel();

  view_.width = view_.width * ratio;
  view_.height = view_.height * ratio;

  if (ratio > 1.0) {
    view_.left -= (mapped_location.x - view_.left) * (ratio - 1.0);
    view_.top -= (mapped_location.y - view_.top) * (ratio - 1.0);
  } else {
    view_.left += (mapped_location.x - view_.left) * ratio;
    view_.top += (mapped_location.y - view_.top) * ratio;
  }

  ClampViewToMaxSize();
}

void CameraView::MovePosition(const sf::Vector2<int32_t> delta) {
  view_.left += delta.x / GetZoomLevel();
  view_.top += delta.y / GetZoomLevel();

  ClampViewToMaxSize();
}

sf::Vector2<double> CameraView::MapPixelToCoords(const sf::Vector2<int32_t> position) const {
  const sf::Vector2<double> camera_position{view_.left, view_.top};
  const sf::Vector2<double> local_position{static_cast<double>(position.x),
                                           static_cast<double>(position.y)};

  return camera_position + local_position / GetZoomLevel();
}

sf::Vector2<int32_t> CameraView::MapCoordsToPixel(const sf::Vector2<double> coords) const {
  const sf::Vector2<double> camera_position{view_.left, view_.top};

  return ToVector2<int32_t>((coords - camera_position) * GetZoomLevel());
}

double CameraView::GetZoomLevel() const {
  return kZoomLevels[zoom_level_];
}

sf::Rect<double> CameraView::GetView() const {
  return view_;
}

void CameraView::ClampViewToMaxSize() {
  view_.left =
      std::clamp(view_.left, 0.0, static_cast<double>(max_size_.x) - view_.width);
  view_.top =
      std::clamp(view_.top, 0.0, static_cast<double>(max_size_.y) - view_.height);
}
