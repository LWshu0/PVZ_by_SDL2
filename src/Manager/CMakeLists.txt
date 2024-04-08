add_library(mapmanager OBJECT MapManager.cpp)
target_link_libraries(mapmanager PUBLIC map)

add_library(plantmanager OBJECT PlantManager.cpp)
target_link_libraries(plantmanager PUBLIC mapmanager)