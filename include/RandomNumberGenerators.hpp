#ifndef RANDOM_NUMBER_GENERATORS_HPP_
#define RANDOM_NUMBER_GENERATORS_HPP_

class FastRng {
  int seed_;

 public:
  FastRng();

  explicit FastRng(int seed);

  int NextValue();
};

template <typename int_type>
class Counter {
  int_type seed_{};

 public:
  Counter() = default;

  explicit Counter(const int_type seed) : seed_(seed) {}

  int_type NextValue() { return seed_++; }

  /**
   * @brief Generate an integer value within [0, max_value).
   * 
   * @param max_value Uppder limit of the interval - excluded.
   * @return int_type Random value in the interval.
   */
  int_type NextInt(const int_type max_value) { return (seed_++) % max_value; }
};

#endif /* RANDOM_NUMBER_GENERATORS_HPP_ */
