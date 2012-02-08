cmake_minimum_required(VERSION 2.6)
project (Typing_of_the_Stars)
set (Typing_of_the_Stars_VERSION_MAJOR 0)
set (Typing_of_the_Stars_VERSION_MINOR 1)

set (TOTS_SOURCE_DIR src)

if(POLICY CMP0015)
  cmake_policy(SET CMP0015 OLD)
endif(POLICY CMP0015)

set(CMAKE_CXX_FLAGS "-g -Wall")

include_directories(SYSTEM ./allegro/addons/color ./allegro/addons/font ./allegro/addons/image ./allegro/addons/primitives ./allegro/addons/ttf ./allegro/include)
link_directories(./allegro/lib)

add_subdirectory("${TOTS_SOURCE_DIR}")
