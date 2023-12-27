#ifndef GAME_ENGINE_HPP_
#define GAME_ENGINE_HPP_

#include <string>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

#include "CameraView.hpp"
#include "EventHandler.hpp"
#include "FallingSandEngine.hpp"
#include "RefreshRate.hpp"
#include "world/Substance.hpp"

class GameEngine {
 public:
  GameEngine() = delete;

  GameEngine(const std::string& application_name, unsigned window_width, unsigned window_height);

  explicit GameEngine(const std::string& application_name);

  void Setup();
  void Run();

  ~GameEngine() = default;

 private:
  void HandleInput();

  void DrawFrame();

  void ComputeNextFrame();

  void ShowDebugInfo();

  void ShowChunkActivity();

  void ShowChunkBorders();

  void DrawBrush();

  void ConstructDebugText(std::string& text_string) const;

  RefreshRate refresh_rate_;
  EventHandler event_handler_;
  sf::RenderWindow window_;
  FallingSandEngine sand_engine_;
  CameraView camera_view_;

  sf::Texture screen_texture_;
  sf::Sprite screen_sprite_;
  std::vector<sf::Uint8> screen_pixels_;

  sf::Text text_;
  sf::Font font_;

  int brush_radius_ = 0;

  engine::Substance chosen_substance_{engine::Substance::kAir};

  unsigned tick_count_{};

  bool do_show_debug_screen_{true};
  bool do_show_chunk_borders_{false};
  bool do_show_chunk_activity_{false};

  bool do_compute_next_frame_{true};
  bool do_advance_one_frame_{false};

  double total_frame_elapsed_time_{};
  double handle_events_elapsed_time_{};
  double screen_update_elapsed_time_{};
  double compute_elapsed_time_{};
};

#endif /* GAME_ENGINE_HPP_ */
