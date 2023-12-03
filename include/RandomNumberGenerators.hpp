#ifndef RANDOM_NUMBER_GENERATORS_HPP_
#define RANDOM_NUMBER_GENERATORS_HPP_

class FastRng {
  int seed_;

 public:
  FastRng();

  explicit FastRng(int seed);

  int NextRandValue();
};

#endif /* RANDOM_NUMBER_GENERATORS_HPP_ */
