#ifndef RENDERING_ENGINE_HPP_
#define RENDERING_ENGINE_HPP_

#include <string>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Text.hpp>

#include "EventHandler.hpp"
#include "RefreshRate.hpp"

class GameEngine {
 public:
  GameEngine() = delete;

  GameEngine(const std::string& application_name, int window_width, int window_height);

  explicit GameEngine(const std::string& application_name);

  void Setup();
  void Run();

  ~GameEngine() = default;

 private:
  void HandleInput();

  void Draw();

  void ShowDebugInfo();

  std::string ConstructDebugText() const;

  std::string application_name_;

  sf::RenderWindow window_;
  RefreshRate refresh_rate_;
  EventHandler event_handler_;

  sf::Texture screen_texture_;
  sf::Sprite screen_sprite_;
  std::vector<sf::Uint8> screen_pixels_;

  sf::Text text_;
  sf::Font font_;

  bool do_show_debug_screen_{};
  bool font_loaded_successfully_{};

  double total_frame_elapsed_time_{};
  double handle_events_elapsed_time_{};
  double screen_update_elapsed_time_{};
};

#endif /* RENDERING_ENGINE_HPP_ */
