cmake_minimum_required(VERSION 3.22)

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
option(USE_OPENCL_FOR_DRAW "Whether to speed up drawing by using OpenCL" ON)
option(GENERATE_DOCUMENTATION "Whether generate Doxygen documentation" OFF)

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

target_link_libraries(${EXECUTABLE_NAME} ${MY_SFML_LIBS})

target_compile_definitions(
  ${EXECUTABLE_NAME}
  PRIVATE -DCL_HPP_ENABLE_EXCEPTIONS
  PRIVATE -DCL_HPP_TARGET_OPENCL_VERSION=${MY_OPENCL_VERSION}
  PRIVATE -DCL_TARGET_OPENCL_VERSION=${MY_OPENCL_VERSION})

if(${USE_OPENCL_FOR_DRAW})
  find_package(OpenCL REQUIRED)

  target_link_libraries(${EXECUTABLE_NAME} OpenCL::OpenCL)

  target_compile_definitions(${EXECUTABLE_NAME} PRIVATE -DUSE_OPENCL_FOR_DRAW)
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(
  SFML ${MY_SFML_VERSION}
  COMPONENTS ${MY_SFML_COMPONENTS}
  REQUIRED)

target_compile_options(${EXECUTABLE_NAME} PRIVATE -Wall -Wextra -Werror
                                                  -pedantic -O3)

file(COPY assets DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

if(${GENERATE_DOCUMENTATION})
  find_package(Doxygen REQUIRED dot mscgen dia)

  set(DOXYGEN_GENERATE_TREEVIEW YES)
  set(DOXYGEN_DISABLE_INDEX NO)
  set(DOXYGEN_FULL_SIDEBAR NO)
  set(DOXYGEN_HTML_EXTRA_STYLESHEET
      /usr/local/share/doxygen-awesome-css/doxygen-awesome.css)
  set(DOXYGEN_HTML_COLORSTYLE LIGHT)
  set(DOXYGEN_GENERATE_HTML YES)
  set(DOXYGEN_HAVE_DOT YES)
  set(DOXYGEN_DOT_IMAGE_FORMAT svg)
  set(DOXYGEN_DOT_TRANSPARENT YES)
  set(DOXYGEN_INTERACTIVE_SVG YES)
  set(DOXYGEN_EXTRACT_ALL YES)
  set(DOXYGEN_CLASS_DIAGRAMS YES)
  set(DOXYGEN_HIDE_UNDOC_RELATIONS NO)
  set(DOXYGEN_CLASS_GRAPH YES)
  set(DOXYGEN_COLLABORATION_GRAPH YES)
  set(DOXYGEN_UML_LOOK YES)
  set(DOXYGEN_UML_LIMIT_NUM_FIELDS50)
  set(DOXYGEN_TEMPLATE_RELATIONS YES)
  set(DOXYGEN_DOT_GRAPH_MAX_NODES 100)
  set(DOXYGEN_MAX_DOT_GRAPH_DEPTH 0)

  doxygen_add_docs(doxygen ALL ${PROJECT_SOURCE_DIR})

  message(STATUS ${DOXYGEN_INPUT})
endif()
