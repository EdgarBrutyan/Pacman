cmake_minimum_required(VERSION 3.0.0)
project(Pacman VERSION 1.0.0)

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

set (CMAKE_CXX_STANDARD 20)

set(SFML_INCLUDE_DIR"C:\\Program Files\\SFML\\include")
message("include dir: ${SFML_INCLUDE_DIR}")
include_directories(${SFML_INCLUDE_DIR})

#message("name: ${PROJECT_NAME}")

add_executable(${PROJECT_NAME} 
            src/main.cpp
            src/settings.hpp
            src/settings.cpp
            src/game_manager.cpp
            src/game_manager.hpp

            src/game_objects/game_object.cpp
            src/game_objects/game_object.hpp
            src/game_objects/movable.cpp
            src/game_objects/movable.hpp
            src/game_objects/pacman.cpp
            src/game_objects/pacman.hpp
            src/game_objects/ghost.cpp
            src/game_objects/ghost.hpp
            src/game_objects/wall.hpp
            src/game_objects/wall.cpp
            src/game_objects/food.hpp
            src/game_objects/food.cpp
            )

target_link_libraries(${PROJECT_NAME} sfml-graphics)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
