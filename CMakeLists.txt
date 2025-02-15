cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)

SET(TARGET vivianite)
project(vivianite)
include_directories(${PROJECT_SOURCE_DIR})

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

#target_sources(${TARGET} PRIVATE main.cpp window.cpp)
target_link_libraries(${TARGET} -g -lraylib)
