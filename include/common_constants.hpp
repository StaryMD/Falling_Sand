#ifndef COMMON_CONSTANTS_HPP_
#define COMMON_CONSTANTS_HPP_

constexpr int kWorldWidth = 1600;
constexpr int kWorldHeight = 800;

constexpr int kChunkSize = 16;
constexpr int kChunkNumWidth = kWorldWidth / kChunkSize;
constexpr int kChunkNumHeight = kWorldHeight / kChunkSize;

constexpr float kWantedFramesPerSecond = 60.0F;
constexpr int kFpsBufferSize = 120;

#endif /* COMMON_CONSTANTS_HPP_ */
