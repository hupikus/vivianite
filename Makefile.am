bin_PROGRAMS = vivianite

AM_CPPFLAGS = \
    -I$(abs_top_srcdir)/include \
    -I$(abs_top_srcdir)/src \
    $(DEPS_CFLAGS)

AM_CXXFLAGS = -std=c++17 -O3


if DEBUG
AM_CXXFLAGS += -Wall
endif

vivianite_SOURCES = \
	src/main.cpp src/argparse.cpp src/window.cpp \
	src/input/keys.cpp \
	src/res/res.cpp src/res/fonts.cpp \
	src/tiling/compositor.cpp src/tiling/tile.cpp \
	src/dl/dlload.cpp src/dl/dllfail.cpp \
	src/render/video.cpp src/render/frame.cpp


vivianite_LDADD = $(DEPS_LIBS)
vivianite_LDADD += -ldl
