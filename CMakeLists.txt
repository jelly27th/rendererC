cmake_minimum_required(VERSION 3.5)
project(Renderer) 

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(MINGW_PATH "C:/MinGW")
set(CMAKE_C_COMPILER "${MINGW_PATH}/bin/gcc")
set(CMAKE_CXX_COMPILER "${MINGW_PATH}/bin/g++")

set(HEADER_FILES
    source/utils/utils.h
    source/matrix/matrix.h
    source/vector/vector.h
    source/color/color.h
    source/point/point.h
    source/framebuffer/framebuffer.h
    source/graphics/graphics.h
    source/win32/win32.h
    source/dyarray/dyarray.h
    source/pmx/pmxFile.h
    source/type/type.h
    source/camera/orbitCamera.h
    source/win32/callback.h
    source/log/log.h
    source/maths/maths.h
    source/bmp/bmpFile.h
    source/thirdparty/lodepng/lodepng.h
)

set(SOURCE_FILES
    source/utils/utils.c
    source/matrix/matrix.c
    source/vector/vector.c
    source/framebuffer/framebuffer.c
    source/graphics/graphics.c
    source/win32/win32.c
    source/pmx/pmxFile.c
    source/camera/orbitCamera.c
    source/win32/callback.c
    source/log/log.c
    source/maths/maths.c
    source/point/point.c
    source/bmp/bmpFile.c
    source/thirdparty/lodepng/lodepng.c
)

set(SRC_CUR main.c)
add_executable(Renderer ${HEADER_FILES} ${SRC_CUR} ${SOURCE_FILES})