#ifndef COMMON_CONSTANTS_HPP_
#define COMMON_CONSTANTS_HPP_

#include <corecrt.h>

constexpr int kWorldWidth = 1600;
constexpr int kWorldHeight = 800;

constexpr int kChunkSize = 16;
constexpr int kChunkNumWidth = kWorldWidth / kChunkSize;
constexpr int kChunkNumHeight = kWorldHeight / kChunkSize;

constexpr double kWantedFramesPerSecond = 60.0F;
constexpr size_t kFpsBufferSize = 120;

constexpr int kDebugRealDigitCount = 3;
constexpr double kDebugRealEpsilon = 1e-7;

#endif /* COMMON_CONSTANTS_HPP_ */
