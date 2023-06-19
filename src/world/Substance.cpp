#include "world/Substance.hpp"

constexpr bool IsSolid(const Substance subs) {
  switch (subs) {
    case Substance::kNothing:
    case Substance::kSubs1:
    case Substance::kSubs2:
    case Substance::kSubs3:
    case Substance::kSubs4:
    case Substance::kSubs5:
    case Substance::kSubs6:
    case Substance::kSubs7:
      return true;
      break;
  }
}
