cmake_minimum_required(VERSION 2.6)
project (Typing_of_the_Stars)

if(POLICY CMP0015)
  cmake_policy(SET CMP0015 OLD)
endif(POLICY CMP0015)

set(CMAKE_CXX_FLAGS "-g -Wall")

include_directories(SYSTEM ./allegro/addons/color ./allegro/addons/font ./allegro/addons/image ./allegro/addons/primitives ./allegro/addons/ttf ./allegro/include)
link_directories(./allegro/lib)

add_executable(tots bitmask.cpp bitmask.h behavior.cpp behavior.h enemy.cpp enemy.h entity.cpp entity.h level.cpp level.h luaFunctions.cpp luaFunctions.h main.cpp movement.cpp movement.h path.cpp path.h player.cpp player.h playerMovement.cpp playerMovement.h sprite.cpp sprite.h world.cpp world.h)
target_link_libraries(tots allegro allegro_image allegro_primitives lua)
