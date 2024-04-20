set(UI_SRC wsShape.cpp wsCircle.cpp wsRectangle.cpp wsTriangles.cpp)
 
add_library(ui OBJECT ${UI_SRC})
target_link_libraries(ui PUBLIC mingw32 SDL2::SDL2main SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf)