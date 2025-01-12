cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 23)

SET(TARGET vivianite)
project(vivianite)
include_directories(${PROJECT_SOURCE_DIR})

#add_subdirectory(bgfx/bx)
#add_subdirectory(bgfx/bimg)
#add_subdirectory(bgfx/bgfx)

set(RENDERER_SOURCE field_renderer.cpp)


set(SOURCES
    main.cpp
    window.cpp
    #editor.cpp
    #field.cpp
    #${RENDERER_SOURCE}
    )


set(HEADERS
    shared_prefs.h
    window.h
    #field.h
    #field_renderer.h
    )

add_executable(${TARGET} ${SOURCES})
#add_executable(${TARGET} main.cpp)

#target_sources(${TARGET} PRIVATE main.cpp window.cpp)
target_link_libraries(${TARGET} -lraylib)
