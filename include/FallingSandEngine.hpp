#ifndef FALLING_SAND_ENGINE_HPP_
#define FALLING_SAND_ENGINE_HPP_

#include <vector>

#include <CL/opencl.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

#include "MasterEngine/CameraView.hpp"
#include "MasterEngine/GameEngine.hpp"
#include "World/Substance.hpp"
#include "World/World.hpp"

class FallingSandEngine : public GameEngine {
 public:
  FallingSandEngine() = delete;

  explicit FallingSandEngine(const std::string& application_name, sf::Vector2<uint32_t> world_size);

  explicit FallingSandEngine(const std::string& application_name, sf::Vector2<uint32_t> world_size,
                             sf::Vector2<uint32_t> screen_size);

  ~FallingSandEngine() override = default;

 private:
  void UserHandleInput() override;
  void UserComputeFrame() override;
  void UserDrawFrame() override;

  void ShowDebugInfo();
  void ShowChunkActivity();
  void ShowChunkBorders();

  void DrawBrush();

  void ConstructDebugText(std::string& text_string) const;

  void PaintOn(const CameraView& camera_view, std::vector<sf::Uint8>& bytes, sf::Vector2<uint32_t> screen_size,
               uint32_t tick_counter);

  void PlaceElementInLine(sf::Vector2<int32_t> start_pos, sf::Vector2<int32_t> end_pos, int32_t radius,
                          engine::Substance substance);

  [[nodiscard]] bool IsChunkActive(sf::Vector2<int32_t> position) const;

  [[nodiscard]] uint32_t GetUpdatedChunksCount() const { return world_.GetChunksUpdatedCount(); }

  [[nodiscard]] World& GetWorld() { return world_; }

  [[nodiscard]] const World& GetWorld() const { return world_; }

  World world_;
  sf::Vector2<uint32_t> screen_size_;

  sf::Texture screen_texture_;
  sf::Sprite screen_sprite_;
  std::vector<sf::Uint8> screen_pixels_;

  cl::Context d_context_;
  cl::CommandQueue d_queue_;
  cl::Buffer d_input_buffer_;
  cl::Buffer d_output_buffer_;
  cl::Kernel d_kernel_;

  void Setup();

  [[nodiscard]] size_t GetPixelCount() const { return static_cast<size_t>(screen_size_.x) * screen_size_.y; }

  sf::Text text_;
  sf::Font font_;

  int32_t brush_radius_ = 0;

  engine::Substance chosen_substance_{engine::Substance::kAir};

  uint32_t tick_count_{};

  bool do_show_debug_screen_{true};
  bool do_show_chunk_borders_{false};
  bool do_show_chunk_activity_{false};

  bool do_compute_next_frame_{true};
  bool do_advance_one_frame_{false};
};

#endif /* FALLING_SAND_ENGINE_HPP_ */
