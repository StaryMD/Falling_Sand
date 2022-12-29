cmake_minimum_required(VERSION 3.22.0)

set(PROJECT Falling_Sand)
project(${PROJECT})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SRC_DIR src)
set(EXEC_NAME runme)
set(CMAKE_CXX_FLAGS "-Ofast")

include(FetchContent)
fetchContent_Declare(
	SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
)
FetchContent_MakeAvailable(SFML)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")
add_executable(${EXEC_NAME} ${SRC_FILES})

target_link_libraries(${EXEC_NAME} PRIVATE sfml-graphics sfml-window sfml-system)
target_compile_features(${EXEC_NAME} PRIVATE cxx_std_20)

target_include_directories(${EXEC_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

install(TARGETS ${EXEC_NAME})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/fonts DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
