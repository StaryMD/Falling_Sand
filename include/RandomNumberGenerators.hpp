#ifndef RANDOM_NUMBER_GENERATORS_HPP_
#define RANDOM_NUMBER_GENERATORS_HPP_

#include <cstdint>

class FastRng {
 public:
  FastRng();

  /**
   * @brief Construct a new RNG object with a specific seed.
   * 
   * @param seed Value of the seed to start at.
   */
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
  /**
  * @brief Construct a new Counter object starting at 0.
  * 
  */
  Counter() = default;

  /**
   * @brief Construct a new Counter object starting at a specific seed.
   * 
   * @param seed 
   */
  explicit Counter(const int_type seed) : seed_(seed) {}

  /**
   * @brief Get the next value at the counter, increase current value.
   * 
   * @return int_type Next value at the counter.
   */
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

#endif  // RANDOM_NUMBER_GENERATORS_HPP_
