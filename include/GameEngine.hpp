#ifndef RENDERING_ENGINE_HPP_
#define RENDERING_ENGINE_HPP_

#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

#include "InputHandler.hpp"
#include "RefreshRate.hpp"

class GameEngine {
 public:
  GameEngine() = delete;

  GameEngine(const std::string& application_name, int window_width, int window_height);

  explicit GameEngine(const std::string& application_name);

  void Setup();
  void Run();

  void HandleEvents();

  void ShowDebugInfo();

  ~GameEngine() = default;

 private:
  std::string application_name_;

  sf::RenderWindow window_;
  RefreshRate<float> refresh_rate_;
  InputHandler input_handler_;

  sf::Event event_;

  sf::Texture screen_texture_;
  sf::Sprite screen_sprite_;
  std::vector<sf::Uint8> screen_pixels_;

  sf::Text fps_text_;
  sf::Font font_;
};

#endif /* RENDERING_ENGINE_HPP_ */
