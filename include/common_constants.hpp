#ifndef COMMON_CONSTANTS_HPP_
#define COMMON_CONSTANTS_HPP_

constexpr int WORLD_WIDTH = 1600;
constexpr int WORLD_HEIGHT = 800;

constexpr int CHUNK_SIZE = 16;
constexpr int CHUNK_NUM_WIDTH = WORLD_WIDTH / CHUNK_SIZE;
constexpr int CHUNK_NUM_HEIGHT = WORLD_HEIGHT / CHUNK_SIZE;

constexpr float WANTED_FRAMES_PER_SECOND = 60.0f;
constexpr int FPS_BUFFER_SIZE = 120;

#endif /* COMMON_CONSTANTS_HPP_ */
