cmake_minimum_required(VERSION 3.23)
project(labwork_7_Kreg101)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(labwork_7_Kreg101 Source/MP3Parser.cpp Header/MP3Parser.h Source/main.cpp Header/Frame.h Source/Frame.cpp Header/Tag.h Source/Tag.cpp)