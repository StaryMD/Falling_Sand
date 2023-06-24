#ifndef COMMON_CONSTANTS_HPP_
#define COMMON_CONSTANTS_HPP_

#include <corecrt.h>

namespace constants {

constexpr int kChunkSize = 16;
constexpr int kChunkNumHorizontal = 200;
constexpr int kChunkNumVertical = 200;

constexpr unsigned kWorldWidth = kChunkSize * kChunkNumHorizontal;
constexpr unsigned kWorldHeight = kChunkSize * kChunkNumVertical;

constexpr double kWantedFramesPerSecond = 60.0F;
constexpr size_t kFpsBufferSize = 120;

constexpr int kDebugDigitPrecision = 3;
constexpr double kDebugRealEpsilon = 1e-7;

constexpr double kWantedSecondsPerFrame = 1.0 / kWantedFramesPerSecond;

}  // namespace constants

#endif /* COMMON_CONSTANTS_HPP_ */
