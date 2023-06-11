#ifndef RANDOM_GENERATORS_HPP_
#define RANDOM_GENERATORS_HPP_

class FastRng {
  int seed_;

 public:
  FastRng();

  int NextRandValue();
};

#endif /* RANDOM_GENERATORS_HPP_ */
