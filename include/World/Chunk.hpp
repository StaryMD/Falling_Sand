#ifndef CHUNK_HPP_
#define CHUNK_HPP_

#include <SFML/System/Vector2.hpp>
#include <array>
#include <cstdint>

class Chunk {
 public:
  /**
   * @brief Construct a empty chunk.
   * 
   */
  Chunk() = default;

  /**
   * @brief Construct a chunk at a certain postion.
   * 
   * @param position Position to create chunk at.
   */
  explicit Chunk(const sf::Vector2<int32_t> position) : position_(position) {}

  /**
   * @brief Check whether the chunk is active.
   * 
   * @param step Step at which the simulation is at - binary value to check the internal array for.
   * @return bool Whether the chunk is active.
   */
  [[nodiscard]] bool IsActive(const int8_t step) const { return activity_[step]; }

  /**
   * @brief Set the activity of the current step.
   * 
   * @param activity Value of the activity flag.
   * @param step Step at which the simulation is at - binary value to check the internal array for.
   */
  void SetActivity(const bool activity, const int8_t step) { activity_[step] = activity; }

 private:
  sf::Vector2<int32_t> position_;
  std::array<bool, 2> activity_;
};

#endif // CHUNK_HPP_
