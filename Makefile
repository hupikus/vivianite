PROJECT_NAME = vivianite


# DESKTOP/ANDROID/WEB
PLATFORM ?= DESKTOP
# WIN/COCOA/X11/WAYLAND
BACKEND ?= WIN

ARCH ?= x86_64

# TRUE/FALSE
UPDATE_SUBMODULES ?= TRUE
DEBUG ?= TRUE

ifeq ($(DEBUG), FALSE)
	UPDATE_SUBMODULES = TRUE
endif

ifeq ($(PLATFORM), DESKTOP)
	CXX ?= g++
	CC ?= gcc
	MAKE = make

	ifeq ($(OS), Windows_NT)
		OS = WINDOWS
		MAKE = mingw32-make
		PROJECT_NAME := $(PROJECT_NAME).exe
	else

		UNAME=$(shell uname)
		ifeq ($(UNAME), Linux)
			OS = LINUX
			BACKEND ?= X11
		endif
		ifeq ($(findstring BSD, $(UNAME)), BSD)
			OS = BSD
			BACKEND = X11

			CXX := clang++
			CC := clang
		endif
		ifeq ($(UNAME), Darwin)
			OS = OSX
			BACKEND = COCOA

			CXX := clang++
			CC := clang
		endif
	endif
endif


BUILD_FOLDER ?= build
OBJ_FOLDER = $(BUILD_FOLDER)/obj

define _obj_name
	$(patsubst %.c,$(OBJ_FOLDER)/%.o,$(patsubst %.cpp,$(OBJ_FOLDER)/%.opp,$(1)))
endef


CC_FLAGS = -O3
CC_INCLUDE = -Iinclude
CC_LINK =

CXX_FLAGS = -O3
#ifeq ($(DEBUG), TRUE)
#	CXX_FLAGS += -fsanitize=address
#endif
ifeq ($(OS), OSX)
	CXX_FLAGS += -std=c++17
endif
CXX_INCLUDE = -Iinclude -Isrc
CXX_LINK = -lboost_filesystem -ldl
CXX_LINK_SDL = -lSDL3 -lSDL3_ttf

ifeq ($(DEBUG), TRUE)
	CC_FLAGS += -Wall
	CXX_FLAGS += -Wall
endif


CC_SOURCES = 
CC_OBJS = $(call _obj_name, $(CC_SOURCES))

CXX_SOURCES =  \
	src/main.cpp src/window.cpp \
	src/input/keys.cpp \
	src/tiling/dlload.cpp src/tiling/tile.cpp \
	src/tiling/compositor.cpp src/tiling/dllfail.cpp \


CXX_OBJS = $(call _obj_name, $(CXX_SOURCES))



APPIMAGE_DIR = $(BUILD_FOLDER)/appimage


.PHONY: all clean _submodules submodules_update appimage

all: _submodules $(PROJECT_NAME)


$(OBJ_FOLDER)/%.opp: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXX_FLAGS) $(CXX_INCLUDE) -c $< -o $@

$(OBJ_FOLDER)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CC_FLAGS) $(CC_INCLUDE) -c $< -o $@

# Build executable
$(PROJECT_NAME): $(CC_OBJS) $(CXX_OBJS)
	$(CXX) -o $@ $(CXX_OBJS) $(CC_OBJS) $(CXX_FLAGS) $(CXX_LINK) $(CXX_LINK_SDL) -D$(PLATFORM) -D$(OS) -D$(BACKEND)


run: all
	./$(PROJECT_NAME)

clean:
ifeq ($(PLATFORM), DESKTOP)
    ifeq ($(OS),WINDOWS)
		rmdir /s /q build
		del $(PROJECT_NAME) || true
    endif
    ifeq ($(OS), $(filter $(OS),LINUX BSD))
		rm -rfv build/*
		rm $(PROJECT_NAME) || true
    endif
    ifeq ($(OS), OSX)
		rm -rf build/*
		rm $(PROJECT_NAME) || true
    endif
endif
	@echo "make clean is not going to remove compiled modules in src/modules!"



CC_INCLUDE += -Iinclude/json.h
CXX_INCLUDE += -Iinclude/json.h
_submodules: submodules_update


submodules_update:
ifeq ($(UPDATE_SUBMODULES), TRUE)
	git submodule update --init --recursive
endif



appimage: all appimage/AppRun appimage/vivianite.desktop
	mkdir -p $(APPIMAGE_DIR)
	mkdir -p $(APPIMAGE_DIR)/usr/share/$(PROJECT_NAME)/
	cp -r assets $(APPIMAGE_DIR)/usr/share/vivianite/

	mkdir -p $(APPIMAGE_DIR)/usr/lib
	for lib in $$(ld $(CXX_LINK) --trace -w | grep '\.so' | tr '\n' ' '); do \
		mkdir -p $(APPIMAGE_DIR)/$$(dirname $${lib}); \
		cp $${lib} $(APPIMAGE_DIR)/$${lib}; \
	done


	cp appimage/AppRun $(APPIMAGE_DIR)/
	cp appimage/vivianite.desktop $(APPIMAGE_DIR)/
	cp $(PROJECT_NAME) $(APPIMAGE_DIR)/usr/share/$(PROJECT_NAME)/

	appimagetool $(APPIMAGE_DIR)


