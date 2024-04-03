
link_libraries(mingw32 SDL2::SDL2main SDL2::SDL2 SDL2_image::SDL2_image)

set(CORE_FILE Camera.cpp Timer.cpp)

add_library(core OBJECT ${CORE_FILE})
