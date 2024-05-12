#ifndef COMMON_CONSTANTS_HPP_
#define COMMON_CONSTANTS_HPP_

#include <cstddef>
#include <cstdint>

namespace constants {

constexpr int32_t kChunkSize = 16;
constexpr int32_t kChunkNumHorizontal = 200;
constexpr int32_t kChunkNumVertical = 100;

constexpr int32_t kWorldWidth = kChunkSize * kChunkNumHorizontal;
constexpr int32_t kWorldHeight = kChunkSize * kChunkNumVertical;

constexpr float kWantedFramesPerSecond = 60.0;
constexpr size_t kFpsBufferSize = 120;

constexpr int32_t kDebugDigitPrecision = 4;
constexpr float kDebugRealEpsilon = 1e-7;

constexpr float kWantedSecondsPerFrame = 1.0 / kWantedFramesPerSecond;

constexpr float kGravityAcceleration = 0.4F;

constexpr float kDefaultVerticalSpeed = 0.01F;
constexpr float kMaxVerticalSpeed = 4.0F;

}  // namespace constants

#endif /* COMMON_CONSTANTS_HPP_ */
