add_library(mapmanager OBJECT MapManager.cpp)
target_link_libraries(mapmanager PUBLIC map)

add_library(productmanager OBJECT ProductManager.cpp)
target_link_libraries(productmanager PUBLIC core product particle)

add_library(plantmanager OBJECT PlantManager.cpp)
target_link_libraries(plantmanager PUBLIC mapmanager)

add_library(zombiemanager OBJECT ZombieManager.cpp)
target_link_libraries(zombiemanager PUBLIC plantmanager)

add_library(taskmanager OBJECT TaskManager.cpp)

add_library(cardmanager OBJECT CardManager.cpp)

add_library(scenemanager OBJECT SceneManager.cpp)

add_library(usrinfomanager OBJECT UsrInfoManager.cpp)

add_library(managers OBJECT Managers.cpp)
