project(babytests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
	../src/CPU.cpp
	../src/RAM.cpp
	tests.cpp
	)

add_executable(babytests ${SOURCES})
