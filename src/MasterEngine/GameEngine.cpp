#include "MasterEngine/GameEngine.hpp"

#include <string>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowStyle.hpp>

#include "CommonConstants.hpp"
#include "MasterEngine/CameraView.hpp"
#include "MasterEngine/RefreshRate.hpp"

GameEngine::GameEngine(const std::string& application_name)
    : window_(sf::VideoMode::getDesktopMode(), application_name, sf::Style::Fullscreen),
      camera_view_({constants::kWorldWidth, constants::kWorldHeight}, window_.getSize(), window_.getSize() / 2U) {}

void GameEngine::Run() {
  refresh_rate_.Reset();
  DrawFrame();

  while (window_.isOpen()) {
    if (refresh_rate_.IsTimeForNewFrame()) {
      refresh_rate_.ResetFrameTime();

      HandleInput();
      ComputeFrame();
      DrawFrame();

      total_frame_elapsed_time_ = refresh_rate_.GetFrameElapsedTime();
    }
  }
}

void GameEngine::DrawFrame() {
  const sf::Clock timer;

  this->UserDrawFrame();

  draw_elapsed_time_ = timer.getElapsedTime().asSeconds();
}

void GameEngine::HandleInput() {
  const sf::Clock timer;

  event_handler_.HandleEvents(window_);

  this->UserHandleInput();

  handle_events_elapsed_time_ = timer.getElapsedTime().asSeconds();
}

void GameEngine::ComputeFrame() {
  const sf::Clock timer;

  this->UserComputeFrame();

  compute_elapsed_time_ = timer.getElapsedTime().asSeconds();
}
