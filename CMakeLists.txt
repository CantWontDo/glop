cmake_minimum_required(VERSION 3.29)
project(glop)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(CMAKE_C_STANDARD 11)
option(TRACY_ENABLE "" ON)

add_subdirectory(src/lib/tracy)
add_subdirectory(src/lib/glfw)

add_executable(
        glop src/main.c
        src/lib/tracy/public/TracyClient.cpp
        src/lib/glad/glad.c
        src/lib/glad/glad.h
        src/lib/glad/KHR/khrplatform.h
        src/err.c
        src/err.h
        src/typedefs.h
        src/glop.c
        src/glop.h
        src/arr.c
        src/arr.h
        src/hash.h
        src/map.c
        src/map.h
)

target_link_libraries(glop PRIVATE glfw)
target_link_libraries(glop PUBLIC TracyClient)