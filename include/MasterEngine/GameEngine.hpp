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

  /**
   * @brief Start and handle the main loop.
   * 
   */
  void Run();

  virtual ~GameEngine() = default;

 protected:
  /**
   * @brief User specified function to handle the input. Must be overriden by the user.
   * 
   */
  virtual void UserHandleInput() = 0;

  /**
   * @brief User specified function to compute the next frame.Must be overriden by the user.
   * 
   */
  virtual void UserComputeFrame() = 0;

  /**
   * @brief User specified function to draw the frame. Must be overriden by the user.
   * 
   */
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
  /**
  * @brief Internal call to the input handler.
  * 
  */
  void HandleInput();

  /**
   * @brief Internal call to compute the next frame.
   * 
   */
  void ComputeFrame();

  /**
   * @brief Internal call to draw the current frame
   * 
   */
  void DrawFrame();
};

#endif  // GAME_ENGINE_HPP_
