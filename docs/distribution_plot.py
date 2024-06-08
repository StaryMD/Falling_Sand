import numpy
import matplotlib.pyplot
from random import random
from scipy import stats

simulations = 1000000
y_limit = 31
cleaver = 5

class UniformRNG0:
  seed = 0

  def unif(self):
    self.seed = 214013 * self.seed + 2531011
    self.seed = (self.seed >> 16) & 0x7FFF
    return self.seed

class UniformRNG1:
  seed = 101

  def unif(self):
    self.seed = 75 * self.seed + 74
    self.seed %= 2**16 + 1
    return self.seed

class CounterRNG:
  seed = 0

  def unif(self):
    self.seed += 1
    return self.seed

rng = UniformRNG1()

x = numpy.arange(0, y_limit)

y = [-1 for _ in range(y_limit)]
u = [-1 for _ in range(y_limit)]

for i in range(simulations):
  rand_value = rng.unif()
  iterations = 1

  while rand_value % cleaver != 0 and iterations < y_limit:
    rand_value = rng.unif()
    iterations += 1
  
  if iterations < y_limit:
    y[iterations] += 1
    

  rand_value = random()
  iterations = 1

  while rand_value > (1 / cleaver) and iterations < y_limit:
    rand_value = random()
    iterations += 1
  
  if iterations < y_limit:
    u[iterations] += 1

print(y)
print(u)

# matplotlib.pyplot.plot(x, y)
matplotlib.pyplot.plot(x[1:], y[1:])
matplotlib.pyplot.show(block=True)

