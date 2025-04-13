cmake_minimum_required(VERSION 3.16)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
SET(TARGET vivianite)
project(vivianite)
include_directories(src)

set(CMAKE_CXX_STANDARD 17)


if(NOT DEFINED COMPILER)

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

if(PLATFORM STREQUAL "windows_x64" OR PLATFORM STREQUAL "windows_x32")
        if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
            set(CMAKE_C_COMPILER "gcc")
            set(CMAKE_CXX_COMPILER "g++")
        else()
            if(PLATFORM STREQUAL "windows_x32")
                set(CMAKE_C_COMPILER "i686-w64-mingw32-gcc")
                set(CMAKE_CXX_COMPILER "i686-w64-mingw32-gcc")
            else()
                set(CMAKE_C_COMPILER "x86_64-w64-mingw32-gcc")
                set(CMAKE_CXX_COMPILER "x86_64-w64-mingw32-g++")
            endif()
        endif()
        set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++ -Wl,-subsystem,windows -mwindows")
elseif(PLATFORM STREQUAL "linux_x64" OR PLATFORM STREQUAL "linux_x32")
    if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        message( FATAL_ERROR "That option is not supported yet." )
    else()
        #if((PLATFORM STREQUAL "linux_x64" AND (CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "x86_64" OR CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "aarch64")) OR (PLATFORM STREQUAL "linux_x32" AND (CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "i686" OR CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "i386")))
        set(CMAKE_C_COMPILER "gcc")
        set(CMAKE_CXX_COMPILER "g++")
        if(PLATFORM STREQUAL "linux_x32")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
        endif()
        #elseif(PLATFORM STREQUAL "linux_x32")

        #else()
            #message( FATAL_ERROR "That option is not supported yet. Try to specify -DCOMPILER and -DRAYLIB_LIB." )
        #endif()
    endif()
elseif(PLATFORM STREQUAL "web")
    if(DEFINED RAYLIB_DIR)
        set(CMAKE_C_COMPILER "emcc")
        set(CMAKE_CXX_COMPILER "em++")
        set(CMAKE_EXECUTABLE_SUFFIX ".html")

        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
    else()
        message( FATAL_ERROR "You didn't specify the raylib.h include folder. Run cmake with -DRAYLIB_DIR=your/path/" )
    endif()

else()
    message( FATAL_ERROR "Unsupported platform '${PLATFORM}'. Choose from linux_x64, linux_x32, windows_x64, windows_x32, web" )
endif()
else()
set(CMAKE_CXX_COMPILER ${COMPILER})
if(NOT DEFINED RAYLIB_LIB)
    message( FATAL_ERROR "You didn't specify the libraylib.a. Run cmake with -DRAYLIB_LIB=path/to/raylib.a" )
endif()
endif()

add_definitions(-O3)

set(SOURCES
    src/globals/resources.cpp
    src/globals/colors.cpp



    src/structure/type/raylib_type.cpp
    src/main.cpp
    src/structure/ui_loop.cpp
    src/structure/compositor.cpp
    src/window.cpp

    #editor
    src/modules/code/editor.cpp
    src/modules/code/field.cpp
    src/modules/code/field_renderer.cpp

    #startscreen
    src/modules/startscreen/startscreen.cpp
    )

#file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")


if(PLATFORM STREQUAL "windows_x64" OR PLATFORM STREQUAL "windows_x32")
    add_executable(${TARGET} ${SOURCES})
    include_directories(${RAYLIB_DIR})
    target_sources(${TARGET} PRIVATE "${CMAKE_SOURCE_DIR}/extra/icon/icon.res")
    if(PLATFORM STREQUAL "windows_x64")
        target_link_libraries(${TARGET} -g ${CMAKE_SOURCE_DIR}/extra/libraylib_win64.a -lopengl32 -lgdi32 -lwinmm)
    else()
        target_link_libraries(${TARGET} -g ${CMAKE_SOURCE_DIR}/extra/libraylib_win32.a stdc++ -lopengl32 -lgdi32 -lwinmm)
    endif()
elseif(PLATFORM STREQUAL "linux_x64" OR PLATFORM STREQUAL "linux_x32")
    add_executable(${TARGET} ${SOURCES})
    #target_link_libraries(${TARGET} -g -lraylib -lpthread)
    if(PLATFORM STREQUAL "linux_x64")
        target_link_libraries(${TARGET} -g ${CMAKE_SOURCE_DIR}/extra/libraylib_linux64.a -lpthread)
    else()
        target_link_libraries(${TARGET} -g ${CMAKE_SOURCE_DIR}/extra/libraylib_linux32.a stdc++ -lpthread)
    endif()
elseif(PLATFORM STREQUAL "web")
    add_executable(${TARGET} ${SOURCES})
    include_directories(${RAYLIB_DIR})
    target_link_libraries(${TARGET} ${CMAKE_SOURCE_DIR}/extra/libraylib_webassembly.a -sUSE_GLFW=3 -sASYNCIFY -sWASM -sFULL_ES2 -sALLOW_MEMORY_GROWTH -sALLOW_TABLE_GROWTH -g0)
    set_target_properties(${TARGET} PROPERTIES LINK_FLAGS "--preload-file ${CMAKE_SOURCE_DIR}/assets@/assets")
else()
    add_executable(${TARGET} ${SOURCES})
    include_directories(${RAYLIB_DIR})
    target_link_libraries(${TARGET} ${RAYLIB_LIB})
endif()
