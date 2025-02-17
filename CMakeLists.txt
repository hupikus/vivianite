cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)


SET(TARGET vivianite)
project(vivianite)
include_directories(${PROJECT_SOURCE_DIR})

if(${PLATFORM} STREQUAL "mingw")
    link_directories("/usr/x86_64-w64-mingw32/include/raylib")
    set(CMAKE_SYSTEM_NAME Windows)
    set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
    set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
    set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")
endif()

add_definitions(-O3)

#set your custom renderers here
set(RENDERER_SOURCE field_renderer.cpp)


set(SOURCES
    raylib_type.cpp
    main.cpp
    window.cpp
    editor.cpp
    field.cpp
    ${RENDERER_SOURCE}
    )


set(HEADERS
    raylib_type.h
    shared_prefs.h
    window.h
    field.h
    field_renderer.h
    )

add_executable(${TARGET} ${SOURCES})

if(${PLATFORM} STREQUAL "mingw")
#target_sources(${TARGET} PRIVATE main.cpp window.cpp)
    target_link_libraries(${TARGET} -g ${CMAKE_SOURCE_DIR}/extra/libraylib.a -lopengl32 -lgdi32 -lwinmm)
else()
    target_link_libraries(${TARGET} -g -lraylib)
endif()
