#ifndef GAME_ENGINE_HPP_
#define GAME_ENGINE_HPP_

#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Window.hpp>

#include "CameraView.hpp"
#include "EventHandler.hpp"
#include "RefreshRate.hpp"

class GameEngine {
 public:
  GameEngine() = delete;

  /**
   * @brief Construct the GameEngine object with a specified window size.
   * 
   * @param application_name Title of the window.
   * @param window_size Size of the window.
   */
  GameEngine(const std::string& application_name, sf::Vector2<uint32_t> window_size);

  /**
   * @brief Construct the GameEngine object with a Fullscreen window.
   * 
   * @param application_name Title of the window.
   */
  explicit GameEngine(const std::string& application_name);

  void Setup();
  void Run();

  virtual ~GameEngine() = default;

 protected:
  virtual void UserHandleInput() = 0;
  virtual void UserComputeFrame() = 0;
  virtual void UserDrawFrame() = 0;

  EventHandler event_handler_;
  sf::RenderWindow window_;
  CameraView camera_view_;

  RefreshRate refresh_rate_;

  double total_frame_elapsed_time_{};
  double handle_events_elapsed_time_{};
  double draw_elapsed_time_{};
  double compute_elapsed_time_{};

 private:
  void HandleInput();

  void ComputeFrame();

  void DrawFrame();
};

#endif /* GAME_ENGINE_HPP_ */
