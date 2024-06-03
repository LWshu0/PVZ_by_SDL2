add_executable(XmlTest XmlTest.cpp)
target_link_libraries(XmlTest PUBLIC mingw32 SDL2::SDL2main SDL2::SDL2 core resource)

add_executable(OperatorTest OperatorTest.cpp)
target_link_libraries(OperatorTest PUBLIC mingw32 SDL2::SDL2main SDL2::SDL2 core )

add_executable(CollectionTest CollectionTest.cpp)
target_link_libraries(CollectionTest PUBLIC core bullet collection collectionmanager)

add_executable(DictionaryTest DictionaryTest.cpp)
target_link_libraries(DictionaryTest PUBLIC core)

add_executable(NormalTest NormalTest.cpp)


add_executable(ParticleTest ParticleTest.cpp)
target_link_libraries(ParticleTest PUBLIC mingw32 SDL2::SDL2main SDL2::SDL2 core particle)
# add_executable(BulletTest BulletTest.cpp)
# target_link_libraries(BulletTest PUBLIC core bullet bulletmanager)

add_executable(ZombieTest ZombieTest.cpp)
target_link_libraries(ZombieTest PUBLIC 
    core 
    map mapmanager 
    bullet bulletmanager 
    plants plantmanager
    zombies zombiemanager
)