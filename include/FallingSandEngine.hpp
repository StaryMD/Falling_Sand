#ifndef FALLING_SAND_ENGINE_HPP_
#define FALLING_SAND_ENGINE_HPP_

#include <vector>

#include <CL/cl2.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

#include "CameraView.hpp"
#include "world/Substance.hpp"
#include "world/World.hpp"

class FallingSandEngine {
 public:
  FallingSandEngine() = delete;

  explicit FallingSandEngine(sf::Vector2<uint32_t> size, sf::Vector2<uint32_t> screen_size);

  void PaintOn(const CameraView& camera_view, std::vector<sf::Uint8>& bytes, sf::Vector2<uint32_t> screen_size,
               uint32_t tick_counter);

  void PlaceElementInLine(sf::Vector2<int32_t> start_pos, sf::Vector2<int32_t> end_pos, int32_t radius,
                          engine::Substance substance);

  void Update();

  [[nodiscard]] bool IsChunkActive(sf::Vector2<int32_t> position) const;

  [[nodiscard]] uint32_t GetUpdatedChunksCount() const { return world_.GetChunksUpdatedCount(); }

  [[nodiscard]] World& GetWorld() { return world_; }

  [[nodiscard]] const World& GetWorld() const { return world_; }

 private:
  World world_;
  sf::Vector2<uint32_t> screen_size_;

  cl::Context d_context_;
  cl::CommandQueue d_queue_;
  cl::Buffer d_input_buffer_;
  cl::Buffer d_output_buffer_;
  cl::Kernel d_kernel_;

  void Setup();

  [[nodiscard]] size_t GetPixelCount() const { return static_cast<size_t>(screen_size_.x) * screen_size_.y; }
};

#endif /* FALLING_SAND_ENGINE_HPP_ */
