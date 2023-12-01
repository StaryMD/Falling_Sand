#ifndef FALLING_SAND_ENGINE_HPP_
#define FALLING_SAND_ENGINE_HPP_

#include <vector>

#include <CL/opencl.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

#include "CameraView.hpp"
#include "world/Substance.hpp"
#include "world/World.hpp"

class FallingSandEngine {
 public:
  FallingSandEngine() = delete;

  explicit FallingSandEngine(sf::Vector2i size, sf::Vector2u screen_size);

  explicit FallingSandEngine(sf::Vector2u size, sf::Vector2u screen_size);

  void PaintOn(const CameraView& camera_view, std::vector<sf::Uint8>& bytes, sf::Vector2u screen_size,
               unsigned tick_counter);

  void PlaceElementInLine(sf::Vector2i start_pos, sf::Vector2i end_pos, engine::Substance substance);

  void Update();

  [[nodiscard]] bool IsChunkActive(sf::Vector2i position) const;

  [[nodiscard]] unsigned GetUpdatedChunksCount() const { return world_.GetChunksUpdatedCount(); }

  [[nodiscard]] World& GetWorld() { return world_; }

  [[nodiscard]] const World& GetWorld() const { return world_; }

 private:
  World world_;
  sf::Vector2u screen_size_;

  cl::Context d_context_;
  cl::CommandQueue d_queue_;
  cl::Buffer d_input_buffer_;
  cl::Buffer d_output_buffer_;
  cl::Kernel d_kernel_;

  void Setup();

  [[nodiscard]] size_t GetPixelCount() const { return static_cast<size_t>(screen_size_.x) * screen_size_.y; }
};

#endif /* FALLING_SAND_ENGINE_HPP_ */
