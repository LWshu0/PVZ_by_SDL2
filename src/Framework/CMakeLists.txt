link_libraries(mingw32 SDL2::SDL2main SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf)

add_executable(PVZ PVZ.cpp)
target_link_libraries(PVZ PUBLIC 
    normal_core sdl_core 
    map mapmanager 
    product productmanager 
    plants plantmanager
    zombies zombiemanager
    taskmanager
    cardmanager
    scene scenemanager
    ui
    usrinfomanager
    particle
)