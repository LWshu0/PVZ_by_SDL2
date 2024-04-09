add_executable(XmlTest XmlTest.cpp)
target_link_libraries(XmlTest PUBLIC normal_core)

add_executable(BulletTest BulletTest.cpp)
target_link_libraries(BulletTest PUBLIC normal_core sdl_core bullet bulletmanager)