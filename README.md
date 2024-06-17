## Falling Sand Simulator

Fallind Sand Simulator is a Cellular Automaton made to simulate real-life physics - as good as it possibly can.<br>

### Current substances:
1. Solids - stone, sand.
2. Fluids - water, oil.
3. Gases - steam, smoke.
4. Outliers - fire.

### How to build:

Configuring is handled by cmake, example for building un Unix-based systems:<br>
```
mkdir build && cd build
cmake ..
ninja
```

* The executable is located at **\<build dir\>/bin/fall**

### Variables for configuring the build:
1. USE_CLANG_TIDY, type boolean, default **OFF**.<br>
  If set to **ON**, will attempt to run the static analyzer tool at build time.<br>
  The tool will attempt use the global executable **clang-tidy**.
2. USE_OPENCL_FOR_DRAW, type boolean, default **ON**.<br>
  If set to **OFF**, will use the slower drawing code written in pure C++.
3. GENERATE_DOCUMENTATION, type boolean, default **OFF**.<br>
  If set tot **ON**, will generate documentation based on the comments written in the code at build time.<br>
  Location of the html file is **\<build dir\>/bin/html/index.html**.

### Requirements:
The project expects the following C++ libraries to be installed at system level
1. SFML 2.5.1.
2. OpenCL 3.0, if drawing with OpenCL is wished.
3. doxygen, dot, mscgen, dia and graphwiz if documentation is wished.