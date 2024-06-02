set(CORE_SRC 
SpinLock.cpp 
Timer.cpp 
Camera.cpp 
GameObject.cpp 
ObjectMotion.cpp 
CoreVars.cpp
)

add_library(core OBJECT ${CORE_SRC})

target_link_libraries(core PUBLIC 
mingw32 
SDL2::SDL2main 
SDL2::SDL2 
SDL2_image::SDL2_image
)

add_subdirectory(UI)

aux_source_directory(Particle PARTICLE_SRC)
add_library(particle OBJECT ${PARTICLE_SRC})
target_link_libraries(particle PUBLIC core)