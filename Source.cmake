# add all source and header files here
set(SOURCE_FILES "")
set(HEADER_FILES "")

# Headers files
set(HEADER_FILES ${HEADER_FILES} source/thirdparty/lodepng/lodepng.h)
set(HEADER_FILES ${HEADER_FILES} source/thirdparty/fast_obj/fast_obj.h)

set(HEADER_FILES ${HEADER_FILES} source/maths/matrix/matrix.h)
set(HEADER_FILES ${HEADER_FILES} source/maths/vector/vector.h)

set(HEADER_FILES ${HEADER_FILES} source/common/type.h)
set(HEADER_FILES ${HEADER_FILES} source/utils/utils.h)
set(HEADER_FILES ${HEADER_FILES} source/color/color.h)
set(HEADER_FILES ${HEADER_FILES} source/point/point.h)
set(HEADER_FILES ${HEADER_FILES} source/framebuffer/framebuffer.h)
set(HEADER_FILES ${HEADER_FILES} source/graphics/graphics.h)
set(HEADER_FILES ${HEADER_FILES} source/win32/win32.h)
set(HEADER_FILES ${HEADER_FILES} source/dyarray/dyarray.h)
set(HEADER_FILES ${HEADER_FILES} source/pmx/pmxFile.h)
set(HEADER_FILES ${HEADER_FILES} source/camera/orbitCamera.h)
set(HEADER_FILES ${HEADER_FILES} source/win32/callback.h)
set(HEADER_FILES ${HEADER_FILES} source/log/log.h)
set(HEADER_FILES ${HEADER_FILES} source/maths/maths.h)
set(HEADER_FILES ${HEADER_FILES} source/bmp/bmpFile.h)
set(HEADER_FILES ${HEADER_FILES} source/png/png.h)

set(HEADER_FILES ${HEADER_FILES} source/renderer/mesh/mesh.h)

set(HEADER_FILES ${HEADER_FILES} source/api.h)

# Source files
set(SOURCE_FILES ${SOURCE_FILES} source/thirdparty/lodepng/lodepng.c)
set(SOURCE_FILES ${HEADER_FILES} source/thirdparty/fast_obj/fast_obj.c)

set(SOURCE_FILES ${SOURCE_FILES} source/maths/matrix/matrix.c)
set(SOURCE_FILES ${SOURCE_FILES} source/maths/vector/vector.c)

set(SOURCE_FILES ${SOURCE_FILES} source/utils/utils.c)
set(SOURCE_FILES ${SOURCE_FILES} source/framebuffer/framebuffer.c)
set(SOURCE_FILES ${SOURCE_FILES} source/graphics/graphics.c)
set(SOURCE_FILES ${SOURCE_FILES} source/win32/win32.c)
set(SOURCE_FILES ${SOURCE_FILES} source/pmx/pmxFile.c)
set(SOURCE_FILES ${SOURCE_FILES} source/camera/orbitCamera.c)
set(SOURCE_FILES ${SOURCE_FILES} source/win32/callback.c)
set(SOURCE_FILES ${SOURCE_FILES} source/log/log.c)
set(SOURCE_FILES ${SOURCE_FILES} source/maths/maths.c)
set(SOURCE_FILES ${SOURCE_FILES} source/point/point.c)
set(SOURCE_FILES ${SOURCE_FILES} source/bmp/bmpFile.c)
set(SOURCE_FILES ${SOURCE_FILES} source/png/png.c)

set(SOURCE_FILES ${SOURCE_FILES} source/renderer/mesh/mesh.c)