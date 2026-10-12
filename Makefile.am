bin_PROGRAMS = vivianite

AM_CPPFLAGS = \
    -I$(abs_top_srcdir)/include \
    -I$(abs_top_srcdir)/src \
    -I$(abs_top_srcdir)/src/include \
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


vivianite_LDFLAGS = -Wl,--export-dynamic
vivianite_LDADD = $(DEPS_LIBS)
vivianite_LDADD += -ldl

DISTCLEANFILES = \
    config.cache \
    config.log \
    config.status \
    configure~ \
    configure.ac~ \
    aclocal.m4 \
    compile \
    config.guess \
    config.sub \
    depcomp \
    install-sh \
    missing \
    ltmain.sh \
    ar-lib \
    test-driver \
    Makefile.in
