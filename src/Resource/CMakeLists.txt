set(RESOURCE_SRC
    XmlLoader.cpp
    TextureRes.cpp 
    Dictionary.cpp
    AnimLoader.cpp
    AnimPlayer.cpp
    ResVars.cpp
    AnimRes.cpp
    DictRes.cpp
)

add_library(resource OBJECT ${RESOURCE_SRC})

target_link_libraries(resource PUBLIC 
mingw32 
SDL2::SDL2main 
SDL2::SDL2 
SDL2_image::SDL2_image 
core
)