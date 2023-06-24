#ifndef COMMON_CONSTANTS_HPP_
#define COMMON_CONSTANTS_HPP_

#include <corecrt.h>

namespace constants {

constexpr unsigned kWorldWidth = 2000;
constexpr unsigned kWorldHeight = 2000;

constexpr int kChunkSize = 16;
constexpr int kChunkNumWidth = kWorldWidth / kChunkSize;
constexpr int kChunkNumHeight = kWorldHeight / kChunkSize;

constexpr double kWantedFramesPerSecond = 60.0F;
constexpr size_t kFpsBufferSize = 120;

constexpr int kDebugDigitPrecision = 3;
constexpr double kDebugRealEpsilon = 1e-7;

constexpr double kWantedSecondsPerFrame = 1.0 / kWantedFramesPerSecond;

}  // namespace constants

#endif /* COMMON_CONSTANTS_HPP_ */
