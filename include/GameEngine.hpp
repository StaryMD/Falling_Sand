#ifndef RENDERING_ENGINE_HPP_
#define RENDERING_ENGINE_HPP_

#include <vector>
#include <string>

#include <SFML/Graphics.hpp>

#include "RefreshRate.hpp"
#include "InputHandler.hpp"

class GameEngine {
 public:
  GameEngine() = delete;
 
  GameEngine(const std::string &application_name, const int window_width, const int window_height);

  GameEngine(const std::string &application_name);

  void setup();
  void run();

  void handle_user_input();
  void handle_events();

  void show_debug_info();

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
