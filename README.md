Motor de jocuri
  Start function
  Game loop:
    Handle input
    Compute
    Draw

Simulator de nisip
  API
  Manages a 2D world:
    Iterate through all substances and update corresponding to the law
  Provides:
    Update function
    Take a screenshot of a certain part of the world
  Optimizations:
    Compute:
      CPU based
      Chunks support - keep track of potential active chunks
      Multi-core support - update multiple chunks at the same time
    Draw:
      GPU based
      OpenCL for the highly parallelizable mapping world to screen

Building the project:
  cmake
  Quality assurance:
    Google C++ coding style
    clang-tidy-18 as static analyser
    clang-format-18 as code formatter

Functia linie
Functia cerc
Functia disc
zoom

Coperta
Cuprins
Introducere
Capitolele lucrarii
  Prezentarea teoriei substantelor
  Prezentarea Bluclei principale
  Implementarea regulilor
  Optimizare
  Quality assurance
Concluzie
Declaratia de autenticicate
