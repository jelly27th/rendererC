cmake_minimum_required(VERSION 3.5)
project(Renderer)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(HEADER_FILES
    vector/vector.h
    color/color.h
    point/point.h
    framebuffer/framebuffer.h
    graphics/graphics.h
    win32/win32.h
)

set(SOURCE_FILES
   framebuffer/framebuffer.c
   graphics/graphics.c
   win32/win32.c
)

set(SRC_CUR main.c)
add_executable(Renderer ${HEADER_FILES} ${SRC_CUR}  
                          ${SOURCE_FILES})