cmake_minimum_required(VERSION 3.16)

if(NOT DEFINED PLATFORM)
    if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
        set(PLATFORM "windows_x64")
        message( "Platform not specified, switching to 'windows_x64'" )
    elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
        set(PLATFORM "linux_x64")
        message( "Platform not specified, switching to 'linux_x64'" )
    else()
        message( FATAL_ERROR "Platform not specified. add -DPLATFORM=windows_x64 to cmake" )
    endif()
else()
    message( "Target platform: '${PLATFORM}'" )
    message( "Host platform: '${CMAKE_HOST_SYSTEM_NAME}'" )
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
SET(TARGET vivianite)
project(vivianite)
include_directories(src)


set(CMAKE_CXX_STANDARD 17)
if(PLATFORM STREQUAL "windows_x64")
        if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
            set(CMAKE_C_COMPILER "gcc")
            set(CMAKE_CXX_COMPILER "g++")
        else()
            set(CMAKE_C_COMPILER "x86_64-w64-mingw32-gcc")
            set(CMAKE_CXX_COMPILER "x86_64-w64-mingw32-g++")
        endif()
        set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++ -Wl,-subsystem,windows -mwindows")
elseif(PLATFORM STREQUAL "linux_x64")
else()
    message( FATAL_ERROR "Unsupported platform '${PLATFORM}'. Choose from linux_x64, windows_x64" )
endif()

add_definitions(-O3)

#set your custom code renderer here
set(CODE_RENDERER_SOURCE src/field_renderer.cpp)


set(SOURCES
    src/raylib_type.cpp
    src/main.cpp
    src/window.cpp
    src/editor.cpp
    src/field.cpp
    ${CODE_RENDERER_SOURCE}
    )


if(PLATFORM STREQUAL "windows_x64")
    add_executable(${TARGET} ${SOURCES})
    target_sources(${TARGET} PRIVATE "${CMAKE_SOURCE_DIR}/extra/icon/icon.res")
    target_link_libraries(${TARGET} -g ${CMAKE_SOURCE_DIR}/extra/libraylib_win64.a -lopengl32 -lgdi32 -lwinmm)
elseif(PLATFORM STREQUAL "linux_x64")
    add_executable(${TARGET} ${SOURCES})
    target_link_libraries(${TARGET} -g -lraylib)
endif()
