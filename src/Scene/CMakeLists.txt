add_library(scene OBJECT 
    MainScene.cpp
    SelectCardScene.cpp
    Select2Game.cpp    
    GameScene.cpp
)
target_link_libraries(scene PUBLIC normal_core sdl_core ui)