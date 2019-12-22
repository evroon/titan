cmake_minimum_required(VERSION 3.5)

if (MSVC)
    add_compile_options(/W4)
else()
    # add_compile_options(-Wall -Wextra -Wfatal-errors)
    add_compile_options(-g)
endif()

include_directories(../src/)
include_directories(/usr/include/bullet/)

file(GLOB sources *.cpp ../src/*/*.cpp ../src/*/*/*.cpp)
add_executable(titan_test ${sources})

set_property(TARGET titan_test PROPERTY CXX_STANDARD 17)

target_link_libraries (titan_test LINK_PUBLIC gtest)
target_link_libraries (titan_test LINK_PUBLIC gtest_main -lpthread -lm)

target_link_libraries (titan_test LINK_PUBLIC SDL2)
target_link_libraries (titan_test LINK_PUBLIC SDL2_image)
target_link_libraries (titan_test LINK_PUBLIC SDL2_ttf)
target_link_libraries (titan_test LINK_PUBLIC SDL2_mixer)
target_link_libraries (titan_test LINK_PUBLIC GLEW)
target_link_libraries (titan_test LINK_PUBLIC GL)
target_link_libraries (titan_test LINK_PUBLIC GLU)
target_link_libraries (titan_test LINK_PUBLIC Box2D)
target_link_libraries (titan_test LINK_PUBLIC assimp)
target_link_libraries (titan_test LINK_PUBLIC noise)
target_link_libraries (titan_test LINK_PUBLIC BulletDynamics)
target_link_libraries (titan_test LINK_PUBLIC BulletCollision)
target_link_libraries (titan_test LINK_PUBLIC LinearMath)