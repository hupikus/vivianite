PROJECT_NAME = vivianite


# DESKTOP/ANDROID/WEB
PLATFORM ?= DESKTOP
# WIN/COCOA/X11/WAYLAND
BACKEND ?= WIN

ifeq ($(PLATFORM), DESKTOP)
	CXX ?= g++
	CC ?= gсс
	MAKE = make

	ifeq ($(OS), Windows_NT)
		OS = WINDOWS
		MAKE = mingw32-make
	else

		UNAME=$(shell uname)
		ifeq ($(UNAME), Linux)
			OS = LINUX
			BACKEND = X11
		endif
		ifeq ($(findstring BSD, $(UNAME)), BSD)
			OS = BSD
			BACKEND = X11

			CXX = clang++
			CC = clang
		endif
		ifeq ($(UNAME), Darwin)
			OS = OSX
			BACKEND = COCOA

			CXX = clang++
			CC = clang
		endif
	endif
endif

#-----------------------------------------------------------------------
# Generate object names from source list.
# All objects are stored in ./build/obj.
# A name of object is made from source path but '\' replaced with '.'.
# For example, src/main.cpp wil become build/obj/src.main.o

BUILD_FOLDER ?= build
OBJ_FOLDER = $(BUILD_FOLDER)/obj/

define _obj_name
	    $(foreach src,$(1),$(OBJ_FOLDER)$(subst .c,.o,$(subst .cpp,.o,$(subst /,.,$(src)))))
endef


# Remove 'build/obj/';
# Replace '.' to '/';
# Add extension to the end

define _src_name
	$(subst .,/,$(patsubst $(OBJ_FOLDER)%.o,%,$(1))).$(2)
endef
#-----------------------------------------------------------------------


CCFLAGS = -Wall -O3
CC_INCLUDE = -Iinclude
CC_LINK =  -lGL -lglfw

CXXFLAGS = -Wall -O3
CXX_INCLUDE = -Iinclude
CXX_LINK =  -lGL -lglfw -lX11


CC_SOURCES = src/include/glad.c
CC_OBJS = $(call _obj_name, $(CC_SOURCES))

CXX_SOURCES = src/main.cpp src/window.cpp
CXX_OBJS = $(call _obj_name, $(CXX_SOURCES))


_objdir = "mkdir" -p $(OBJ_FOLDER)

all: $(PROJECT_NAME)


$(CXX_OBJS): $(OBJ_FOLDER)%.o:
	$(call _objdir)
	$(CXX) $(CXXFLAGS) $(CXX_INCLUDE) -c $(call _src_name, $@,cpp) -o $@

$(CC_OBJS): $(OBJ_FOLDER)%.o:
	$(call _objdir)
	$(CC) $(CCFLAGS) $(CC_INCLUDE) -c $(call _src_name, $@,c) -o $@

$(PROJECT_NAME): $(CXX_OBJS) $(CC_OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(CXX_LINK) -D$(PLATFORM) -D$(OS) -D$(BACKEND)

run: all
	./$(PROJECT_NAME)

clean:
ifeq ($(PLATFORM), DESKTOP)
    ifeq ($(OS),WINDOWS)
		rmdir /s /q build
		del $(PROJECT_NAME).exe
    endif
    ifeq ($(OS), LINUX)
		rm -rfv build/*
		rm $(PROJECT_NAME)
    endif
    ifeq ($(OS), OSX)
		rm -f build/*
		rm $(PROJECT_NAME)
    endif
endif
