cmake_minimum_required(VERSION 3.24)

project(Lonesome-Island)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_PREFIX_PATH "${PROJECT_SOURCE_DIR}")

set(CMAKE_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/include")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

set(SDL2_DIR "${PROJECT_SOURCE_DIR}")
set(SDL2_IMAGE_DIR "${PROJECT_SOURCE_DIR}")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

set(SDL2_LIBRARIES "-L${SDL2_LIBRARY_DIRS} -lSDL2 -lSDL2main")
set(SDL2_IMAGE_LIBRARIES "-L${SDL2_IMAGE_LIBRARY_DIRS} -lSDL2_image")

add_executable(Lonesome-Island src/main.c src/client.c src/defs.c src/menus.c src/socket.c src/settings.c src/create_map.c src/map.c src/defs.c src/sound.c)

target_link_libraries(Lonesome-Island ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
