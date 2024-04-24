add_executable(XmlTest XmlTest.cpp)
target_link_libraries(XmlTest PUBLIC normal_core)

add_executable(OperatorTest OperatorTest.cpp)
target_link_libraries(OperatorTest PUBLIC mingw32 SDL2::SDL2main SDL2::SDL2 normal_core sdl_core )

# add_executable(BulletTest BulletTest.cpp)
# target_link_libraries(BulletTest PUBLIC normal_core sdl_core bullet bulletmanager)

# add_executable(ZombieTest ZombieTest.cpp)
# target_link_libraries(ZombieTest PUBLIC 
#     normal_core sdl_core 
#     map mapmanager 
#     bullet bulletmanager 
#     plants plantmanager
#     zombies zombiemanager
# )