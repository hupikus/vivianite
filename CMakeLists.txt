cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 23)

SET(TARGET vivianite)
project(vivianite)
include_directories(${PROJECT_SOURCE_DIR})


find_package(SFML 2.5 REQUIRED
            graphics
            window
            system)


set(RENDERER_SOURCE field_renderer.cpp)

set(SOURCES
    window.cpp
    editor.cpp
    main.cpp
    field.cpp
    ${RENDERER_SOURCE}
    )

set(HEADERS
    shared_prefs.h
    window.h
    field.h
    field_renderer.h
    )



add_executable(${TARGET} ${SOURCES})

target_sources(${TARGET} PRIVATE editor.cpp ${RENDERER_SOURCE})
target_link_libraries(${TARGET} PRIVATE sfml-graphics sfml-window sfml-system)
