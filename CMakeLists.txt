cmake_minimum_required(VERSION 3.5...3.11)
project(Renderer) 

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# set(MINGW_PATH "C:/MinGW")
# set(CMAKE_C_COMPILER "${MINGW_PATH}/bin/gcc")
# set(CMAKE_CXX_COMPILER "${MINGW_PATH}/bin/g++")

include(Source.cmake)

set(SRC_CUR main.c)
add_executable(Renderer ${HEADER_FILES} ${SRC_CUR} ${SOURCE_FILES})