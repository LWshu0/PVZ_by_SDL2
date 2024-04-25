add_library(mapmanager OBJECT MapManager.cpp)
target_link_libraries(mapmanager PUBLIC map)

add_library(bulletmanager OBJECT BulletManager.cpp)
target_link_libraries(bulletmanager PUBLIC sdl_core bullet)

add_library(plantmanager OBJECT PlantManager.cpp)
target_link_libraries(plantmanager PUBLIC mapmanager)

add_library(zombiemanager OBJECT ZombieManager.cpp)
target_link_libraries(zombiemanager PUBLIC plantmanager)

add_library(collectionmanager OBJECT CollectionManager.cpp)

add_library(taskmanager OBJECT TaskManager.cpp)

add_library(cardmanager OBJECT CardManager.cpp)

add_library(scenemanager OBJECT SceneManager.cpp)