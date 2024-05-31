add_library(product OBJECT
ProductObject.cpp
Pea.cpp
Sun.cpp
)

target_link_libraries(product PUBLIC particle)