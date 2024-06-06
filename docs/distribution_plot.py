import numpy
import matplotlib.pyplot
from scipy import stats

simulations = 100000
y_limit = 50
cleaver = 5

class UniformRNG:
  seed = 0

  def unif(self):
    self.seed = 214013 * self.seed + 2531011
    self.seed = (self.seed >> 16) & 0x7FFF
    return self.seed

class CounterRNG:
  seed = 0

  def unif(self):
    self.seed += 1
    return self.seed

rng = CounterRNG()

x = numpy.arange(0, y_limit)

y = [0 for _ in range(y_limit)]

for i in range(simulations):
  rand_value = rng.unif()
  iterations = 1

  while rand_value % cleaver != 0:
    rand_value = rng.unif()
    iterations += 1
  
  if iterations < y_limit:
    y[iterations] += 1

print(y)

matplotlib.pyplot.plot(x, y)
matplotlib.pyplot.show(block=True)

