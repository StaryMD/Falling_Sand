Motor de jocuri
  Start function
  Game loop:
    Handle input
    Compute
    Draw

Simulator de nisip
  API
  Manages a 2D world:
    Iterate throuhg all substances and update corresponding to the law
  Provides:
    Update function
    Take a screenshot of a certain part of the world
  Optimizations:
    Compute:
      CPU based
      Chunks support - keep track of potantial active chunks
      Multi-core support - update multiple chunks at the same time
    Draw:
      GPU based
      OpenCL for the highly parallelizable mapping world - screen

Building the project:
  cmake + preferred generator
  Quality assurance:
    clang-tidy-18 as static analyser
    clang-format-18 as code formatter
