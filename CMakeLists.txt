cmake_minimum_required(VERSION 3.16)
project(PhysicsEngine CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(raylib)

add_executable(PhysicsEngine
    src/main.cpp
    src/particle.cpp
)

target_include_directories(PhysicsEngine PRIVATE include)
target_link_libraries(PhysicsEngine PRIVATE raylib)