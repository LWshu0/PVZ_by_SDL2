# 指定编译器
set (CMAKE_C_COMPILER "D:/compliers/mingw64_posix_seh/bin/gcc.exe")
set (CMAKE_CXX_COMPILER "D:/compliers/mingw64_posix_seh/bin/g++.exe")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# 设置编译器选项
# set(CMAKE_CXX_FLAGS   "-lmingw32 -lSDL2main -lSDL2")  
# set(CMAKE_C_FLAGS   "-lmingw32 -lSDL2main -lSDL2")  

# cmake 最低版本
cmake_minimum_required(VERSION 3.27)
# 项目名称
project(PVZ)

# 使用 .exe 运行时静态链接
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")

# 定义路径
get_filename_component(PROJECT_DIR ${CMAKE_CURRENT_LIST_FILE} DIRECTORY)
set(INCLUDE_DIR ${PROJECT_DIR}/include)
set(RUN_DIR ${PROJECT_DIR}/run)
# 源文件路径
set(PROJECT_SOURCE_DIR ${PROJECT_DIR}/src)
# 可执行文件输出位置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${RUN_DIR}/bin)
# 动态库输出位置
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${RUN_DIR}/bin)
# 静态库输出位置
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${RUN_DIR}/lib)

# message(${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
# message(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
# message(${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
# message(${PROJECT_SOURCE_DIR})
# message(${PROJECT_BINARY_DIR})

list(APPEND CMAKE_PREFIX_PATH "D:/3rd_lib")

find_package(SDL2 REQUIRED)
if(SDL2_FOUND)
    message(STATUS "Find SDL2")
    foreach(VALUE IN LISTS SDL2_INCLUDE_DIRS)
        message(STATUS "SDL_INCLUDE PATH: ${VALUE}")
    endforeach()
endif()

find_package(SDL2_image REQUIRED)
if(SDL2_image_FOUND)
    message(STATUS "Find SDL2_image")
    foreach(VALUE IN LISTS SDL2_image_INCLUDE_DIRS)
        message(STATUS "SDL2_image_INCLUDE PATH: ${VALUE}")
    endforeach()
endif()

find_package(SDL2_ttf REQUIRED)
if(SDL2_ttf_FOUND)
    message(STATUS "Find SDL2_ttf")
    foreach(VALUE IN LISTS SDL2_ttf_INCLUDE_DIRS)
        message(STATUS "SDL2_ttf_INCLUDE PATH: ${VALUE}")
    endforeach()
endif()

find_package(glm REQUIRED)
if(glm_FOUND)
    message(STATUS "Find glm")
    foreach(VALUE IN LISTS GLM_INCLUDE_DIRS)
        message(STATUS "GLM_INCLUDE_DIRS PATH: ${VALUE}")
    endforeach()
endif()

# 指明所有头文件路径
include_directories(${INCLUDE_DIR}
                    ${SDL2_INCLUDE_DIRS}
                    ${SDL2_image_INCLUDE_DIRS}
                    ${SDL2_ttf_INCLUDE_DIRS}
                    ${GLM_INCLUDE_DIRS}
                    )
# 链接库文件
link_directories(${RUN_DIR}/lib
                 ${RUN_DIR}/bin)

add_subdirectory(
    ${PROJECT_SOURCE_DIR}
)