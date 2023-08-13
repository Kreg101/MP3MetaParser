cmake_minimum_required(VERSION 3.23)
project(parser)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(parser Source/MP3Parser.cpp Header/MP3Parser.h bin/main.cpp Header/Frame.h Source/Frame.cpp Header/Tag.h Source/Tag.cpp)