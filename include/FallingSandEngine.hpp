#ifndef FALLING_SAND_ENGINE_HPP_
#define FALLING_SAND_ENGINE_HPP_

#include <vector>

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

#include "MasterEngine/CameraView.hpp"
#include "MasterEngine/GameEngine.hpp"
#include "World/Substance.hpp"
#include "World/World.hpp"

#ifdef USE_OPENCL_FOR_DRAW

#include <CL/opencl.hpp>

#endif  // USE_OPENCL_FOR_DRAW

class FallingSandEngine : public GameEngine {
 public:
  FallingSandEngine() = delete;

  /**
   * @brief Construct a new Falling Sand Engine from the application name and world size, assumes fullscreen.
   * 
   * @param[in] application_name Name of the application - showed in the title bar.
   * @param[in] world_size Size of the world to be simulated.
   */
  explicit FallingSandEngine(const std::string& application_name, sf::Vector2<uint32_t> world_size);

  /**
   * @brief Construct a new Falling Sand Engine from the application name and world size, assumes fullscreen.
   * 
   * @param[in] application_name Name of the application - showed in the title bar.
   * @param[in] world_size Size of the world to be simulated.
   * @param[in] screen_size Size of the screen.
   */
  explicit FallingSandEngine(const std::string& application_name, sf::Vector2<uint32_t> world_size,
                             sf::Vector2<uint32_t> screen_size);

  ~FallingSandEngine() override = default;

 private:
  /**
   * @brief Personalized function to handle input.
   * 
   */
  void UserHandleInput() override;

  /**
   * @brief Personalized function to compute the next frame.
   * 
   */
  void UserComputeFrame() override;

  /**
   * @brief Personalized function to draw the frame.
   * 
   */
  void UserDrawFrame() override;

  /**
   * @brief Show the debug info for this frame.
   * 
   */
  void ShowDebugInfo();

  /**
   * @brief Show the activity of the chunks in this frame.
   * 
   */
  void ShowChunkActivity();

  /**
   * @brief Show the border of the chunks in this frame.
   * 
   */
  void ShowChunkBorders();

  /**
   * @brief Draw the brush to the screen.
   * 
   */
  void DrawBrush();

  /**
   * @brief Construct the debug text for the **ShowDebugInfo** function.
   * 
   * @param[out] text_string Debug info to display.
   */
  void ConstructDebugText(std::string& text_string) const;

  /**
   * @brief Populate the vector of pixels with a screenshot of the world at the given location.
   * 
   * @param[in] camera_view Camera view to extract the frustum from.
   * @param[out] bytes 2D pixel array to store the screenshot to.
   * @param[in] screen_size Size of the 2d pixel array.
   * @param[in] tick_counter Tick counter - used in animating the elements.
   */
  void PaintOn(const CameraView& camera_view, std::vector<sf::Uint8>& bytes,
               sf::Vector2<uint32_t> screen_size, uint32_t tick_counter);

  /**
   * @brief Place a bunch of elements in a line.
   * 
   * @param[in] start_pos Starting postion of the line.
   * @param[in] end_pos End postion of the line.
   * @param[in] radius If not 1, a disc will be placed at every pixel in the line.
   * @param[in] substance Underlying substance of all the elements to be placed.
   */
  void PlaceElementInLine(sf::Vector2<int32_t> start_pos, sf::Vector2<int32_t> end_pos,
                          int32_t radius, engine::Substance substance);

  /**
   * @brief Check whether a chunk is active.
   * 
   * @param[in] position Position of the chunk.
   * @return bool Whether the chunk is a active.
   */
  [[nodiscard]] bool IsChunkActive(sf::Vector2<int32_t> position) const;

  /**
   * @brief Get the Updated Chunks Count object
   * 
   * @return uint32_t 
   */
  [[nodiscard]] uint32_t GetUpdatedChunksCount() const { return world_.GetChunksUpdatedCount(); }

  /**
   * @brief Get a reference to the world simulated.
   * 
   * @return World& The simulated world.
   */
  [[nodiscard]] World& GetWorld() { return world_; }

  /**
   * @brief Get a immutable reference to the world.
   * 
   * @return World& A immutable reference to the simulated world.
   */
  [[nodiscard]] const World& GetWorld() const { return world_; }

  World world_;
  sf::Vector2<uint32_t> screen_size_;

  sf::Texture screen_texture_;
  sf::Sprite screen_sprite_;
  std::vector<sf::Uint8> screen_pixels_;

#ifdef USE_OPENCL_FOR_DRAW

  cl::Context d_context_;
  cl::CommandQueue d_queue_;
  cl::Buffer d_input_buffer_;
  cl::Buffer d_output_buffer_;
  cl::Kernel d_kernel_;

#endif  // USE_OPENCL_FOR_DRAW

  void Setup();

  /**
   * @brief Get the number of pixels in the screen.
   * 
   * @return size_t Number of pixels in the screen.
   */
  [[nodiscard]] size_t GetPixelCount() const {
    return static_cast<size_t>(screen_size_.x) * screen_size_.y;
  }

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

#endif  // FALLING_SAND_ENGINE_HPP_
