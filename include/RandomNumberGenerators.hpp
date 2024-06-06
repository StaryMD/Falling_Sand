#ifndef RANDOM_NUMBER_GENERATORS_HPP_
#define RANDOM_NUMBER_GENERATORS_HPP_

#include <cstdint>

class FastRng {
 public:
  FastRng();

  explicit FastRng(int32_t seed);

  /**
   * @brief Generate a random number in the interval [0, 32767).
   * 
   * @return int32_t Random number in the interval [0, 32767).
   */
  int32_t NextValue();

 private:
  int32_t seed_;
};

template <typename int_type>
class Counter {
 public:
  Counter() = default;

  explicit Counter(const int_type seed) : seed_(seed) {}

  int_type NextValue() { return seed_++; }

  /**
   * @brief Generate an integer value within [0, max_value).
   * 
   * @param max_value Upper limit of the interval - excluded.
   * @return int_type Random value in the interval.
   */
  int_type NextInt(const int_type max_value) { return (seed_++) % max_value; }

 private:
  int32_t seed_{};
};

#endif /* RANDOM_NUMBER_GENERATORS_HPP_ */
