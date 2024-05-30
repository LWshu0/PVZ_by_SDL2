set(NORMAL_CORE_SRC SpinLock.cpp XmlLoader.cpp)
add_library(normal_core OBJECT ${NORMAL_CORE_SRC})

set(SDL_CORE_SRC Timer.cpp Camera.cpp TextureRes.cpp Dictionary.cpp AnimLoader.cpp AnimPlayer.cpp GameObject.cpp ObjectMotion.cpp GlobalVars.cpp)
add_library(sdl_core OBJECT ${SDL_CORE_SRC})
target_link_libraries(sdl_core PUBLIC mingw32 SDL2::SDL2main SDL2::SDL2 SDL2_image::SDL2_image)

add_subdirectory(UI)

aux_source_directory(Particle PARTICLE_SRC)
add_library(particle OBJECT ${PARTICLE_SRC})