#ifndef COMMON_CONSTANTS_HPP_
#define COMMON_CONSTANTS_HPP_

#include <cstddef>

namespace constants {

constexpr int kChunkSize = 16;
constexpr int kChunkNumHorizontal = 200;
constexpr int kChunkNumVertical = 100;

constexpr int kWorldWidth = kChunkSize * kChunkNumHorizontal;
constexpr int kWorldHeight = kChunkSize * kChunkNumVertical;

constexpr double kWantedFramesPerSecond = 60.0;
constexpr size_t kFpsBufferSize = 120;

constexpr int kDebugDigitPrecision = 4;
constexpr double kDebugRealEpsilon = 1e-7;

constexpr double kWantedSecondsPerFrame = 1.0 / kWantedFramesPerSecond;

constexpr float kGravityAcceleration = 0.4F;

}  // namespace constants

#endif /* COMMON_CONSTANTS_HPP_ */
