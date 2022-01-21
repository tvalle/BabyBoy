cmake_minimum_required(VERSION 3.10)

project(BabyBoy)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
	src/BabyBoy.cpp
	src/CPU.cpp
	src/RAM.cpp
	src/Rom.cpp
	src/SDL/SDLWindow.cpp
	src/SoC.cpp
	src/Config.cpp
	src/windows/MainWindowDesktop.cpp
	src/State.cpp)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(BabyBoy ${SOURCES})
target_link_libraries(BabyBoy ${SDL2_LIBRARIES})
