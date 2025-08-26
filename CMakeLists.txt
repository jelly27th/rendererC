cmake_minimum_required(VERSION 3.5)
project(Renderer)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(HEADER_FILES
    utils/utils.h
    matrix/matrix.h
    vector/vector.h
    color/color.h
    point/point.h
    framebuffer/framebuffer.h
    graphics/graphics.h
    win32/win32.h
    dyarray/dyarray.h
    pmx/pmxFile.h
    type/type.h
    camera/orbitCamera.h
    win32/callback.h
    log/log.h
)

set(SOURCE_FILES
    utils/utils.c
    matrix/matrix.c
    vector/vector.c
    framebuffer/framebuffer.c
    graphics/graphics.c
    win32/win32.c
    pmx/pmxFile.c
    camera/orbitCamera.c
    win32/callback.c
    log/log.c
)

set(SRC_CUR main.c)
add_executable(Renderer ${HEADER_FILES} ${SRC_CUR} ${SOURCE_FILES})