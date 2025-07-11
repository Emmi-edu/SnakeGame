cmake_minimum_required(VERSION 3.25)
project(GreedySnake)
set(SFML_DIR "C:/Users/emmit/CLionProjects/CPPSFML/SFML/lib/cmake/SFML")
find_package(SFML COMPONENTS graphics window system REQUIRED)
set(CMAKE_CXX_STANDARD 17)
include_directories(header)
file(GLOB SOURCES "src/*.cpp")
add_executable(GreedySnake ${SOURCES})
target_link_libraries(GreedySnake
        sfml-graphics
        sfml-window
        sfml-system
        )
