#include "world/Substance.hpp"

constexpr bool IsSolid(const Substance subs) {
  switch (subs) {
    case Substance::kNothing:
    case Substance::kSand:
      return true;
  }
}
