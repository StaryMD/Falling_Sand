cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(EXECUTABLE_NAME fall)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS
  ON
  CACHE INTERNAL "")
set(CMAKE_COLOR_DIAGNOSTICS
  ON
  CACHE BOOL "")
set(CMAKE_COLOR_MAKEFILE
  ON
  CACHE BOOL "")

set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_COMPILER_LAUNCHER "ccache")

set(MY_CPP_VERSION
  17
  CACHE STRING "")
set(MY_OPENCL_VERSION
  300
  CACHE STRING "")
set(MY_SFML_VERSION
  2.5.1
  CACHE STRING "")

set(MY_SFML_COMPONENTS graphics window system)
set(MY_SFML_LIBS sfml-graphics sfml-window sfml-system)

project(FallingSand)

option(USE_CLANG_TIDY "Whether to use clang-tidy for static analysis" OFF)

if(${USE_CLANG_TIDY})
  set(CMAKE_CXX_CLANG_TIDY
    clang-tidy; -format-style='file'; -extra-arg=-std=c++${MY_CPP_VERSION};
    --quiet; --use-color; -p=${CMAKE_BINARY_DIR}/compile_commands.json;)
endif()

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADER_FILES ${CMAKE_SOURCE_DIR}/include/*.hpp)

add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES} ${HEADER_FILES})

set_property(TARGET ${EXECUTABLE_NAME} PROPERTY CXX_STANDARD ${MY_CPP_VERSION})
set_property(TARGET ${EXECUTABLE_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET ${EXECUTABLE_NAME} PROPERTY CXX_EXTENSIONS OFF)

target_include_directories(${EXECUTABLE_NAME}
  PRIVATE ${CMAKE_SOURCE_DIR}/include/)

find_package(OpenCL)
if(OpenCL_FOUND)
  target_link_libraries(${EXECUTABLE_NAME} OpenCL::OpenCL Threads::Threads ${MY_SFML_LIBS})
  target_compile_definitions(
    ${EXECUTABLE_NAME}
    PRIVATE -DUSE_OPENCL_FOR_DRAW
    PRIVATE -DCL_HPP_ENABLE_EXCEPTIONS
    PRIVATE -DCL_HPP_TARGET_OPENCL_VERSION=${MY_OPENCL_VERSION}
    PRIVATE -DCL_TARGET_OPENCL_VERSION=${MY_OPENCL_VERSION})
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
# target_link_libraries(${EXECUTABLE_NAME} PRIVATE Threads::Threads)

# set(SFML_STATIC_LIBRARIES TRUE)
find_package(
  SFML ${MY_SFML_VERSION}
  COMPONENTS ${MY_SFML_COMPONENTS}
  REQUIRED)

target_compile_options(${EXECUTABLE_NAME} PRIVATE -Wall -Wextra -Werror
  -pedantic -O3)
file(COPY assets DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
