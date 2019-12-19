cmake_minimum_required(VERSION 3.5)

if (MSVC)
    add_compile_options(/W4)
else()
    # add_compile_options(-Wall -Wextra -Wfatal-errors)
    add_compile_options(-g)
endif()

include_directories(./)

file(GLOB sources */*.cpp */*/*.cpp)
add_executable(titan ${sources})

set_property(TARGET titan PROPERTY CXX_STANDARD 17)

target_link_libraries (titan LINK_PUBLIC SDL2)
target_link_libraries (titan LINK_PUBLIC SDL2_image)
target_link_libraries (titan LINK_PUBLIC SDL2_ttf)
target_link_libraries (titan LINK_PUBLIC SDL2_mixer)
target_link_libraries (titan LINK_PUBLIC GLEW)
target_link_libraries (titan LINK_PUBLIC GL)
target_link_libraries (titan LINK_PUBLIC GLU)
target_link_libraries (titan LINK_PUBLIC Box2D)
target_link_libraries (titan LINK_PUBLIC assimp)
target_link_libraries (titan LINK_PUBLIC noise)
