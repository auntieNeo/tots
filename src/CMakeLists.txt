add_executable(tots bitmask.cpp bitmask.h behavior.cpp behavior.h enemy.cpp enemy.h entity.cpp entity.h level.cpp level.h luaFunctions.cpp luaFunctions.h main.cpp movement.cpp movement.h path.cpp path.h player.cpp player.h playerMovement.cpp playerMovement.h sprite.cpp sprite.h world.cpp world.h)
target_link_libraries(tots allegro allegro_image allegro_primitives lua)
