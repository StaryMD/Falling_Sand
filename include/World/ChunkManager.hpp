#ifndef CHUNK_MANAGER_HPP_
#define CHUNK_MANAGER_HPP_

#include <vector>

#include <SFML/System/Vector2.hpp>

#include "World/Chunk.hpp"

class ChunkManager {
 public:
  ChunkManager() = delete;

  /**
   * @brief Construct a ChunkManager.
   * 
   * @param size Vector Holding the size of the chunks to be managed.
   */
  explicit ChunkManager(sf::Vector2<int32_t> size);

  /**
   * @brief Get the size of the chunk manager.
   * 
   * @return sf::Vector2<int32_t> Size of the chunk manager.
   */
  [[nodiscard]] sf::Vector2<int32_t> GetSize() const { return size_; };

  /**
   * @brief Check whether a chunk is active.
   * 
   * @param pos_index Position in a 2D array of the chunk to be checked.
   * @return bool Whether a chunk is active.
   */
  [[nodiscard]] bool IsActive(int32_t pos_index) const;

  /**
   * @brief Check whether a chunk is active.
   * 
   * @param pos_index Coordinates of the chunk to be checked.
   * @return bool Whether a chunk is active.
   */
  [[nodiscard]] bool IsActive(sf::Vector2<int32_t> position) const;

  /**
   * @brief Set the activity of a certain chunk.
   * 
   * @param position Coordinates of the chunk.
   * @param activity_ Value of the activity flag.
   */
  void SetNextStepActivity(sf::Vector2<int32_t> position, bool activity_ = true);

  /**
   * @brief Swap the to internal buffers of the chunks, determining a new step in the simulation.
   * 
   */
  void SwapBuffers();

 private:
  sf::Vector2<int32_t> size_;

  std::vector<Chunk> chunks_;
  int8_t current_step_{};
};

#endif // CHUNK_MANAGER_HPP_
