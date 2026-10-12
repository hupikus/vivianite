PROJECT_NAME = vivianite
bin_PROGRAMS = $(PROJECT_NAME)

BUILDDIR = $(abs_builddir)
OBJDIR = $(BUILDDIR)

AM_CPPFLAGS = \
    -I$(abs_top_srcdir)/include \
-I$(abs_top_srcdir)/src \
    -I$(abs_top_srcdir)/src/include \
    $(DEPS_CFLAGS)

AM_CXXFLAGS = -std=c++17 -O3


if DEBUG
AM_CXXFLAGS += -Wall
endif

vivianite_LDFLAGS = -Wl,--export-dynamic
vivianite_LDADD = $(DEPS_LIBS)
vivianite_LDADD += -ldl


CC_SRC = 
CXX_SRC = \
	src/main.cpp src/argparse.cpp src/window.cpp \
	src/input/keys.cpp \
	src/res/res.cpp src/res/fonts.cpp \
	src/tiling/compositor.cpp src/tiling/tile.cpp \
	src/dl/dlload.cpp src/dl/dllfail.cpp \
	src/render/video.cpp src/render/frame.cpp



CC_OBJS = $(patsubst %.c,$(OBJDIR)/%.o,$(CC_SRC))
CXX_OBJS = $(patsubst %.cpp,$(OBJDIR)/%.o,$(CXX_SRC))

vivianite_SOURCES = $(CXX_SRC)
override vivianite_OBJECTS = $(CXX_OBJS)



SRC_COUNT = $(words $(CXX_SRC))
COMPILED_FILES =

am__v_CXX_0 = \
    $(eval COMPILED_FILES += $<) \
    $(eval COMPILED_FILES_COUNT = $(words $(COMPILED_FILES))) \
    @echo "[$(COMPILED_FILES_COUNT)/$\$(SRC_COUNT)] CXX $<" && 


MODULES =   modules/builtin/startscreen \
	    modules/builtin/buffer \
	    modules/builtin/split \
	    modules/quick-render


.PHONY: modules
modules:
	@echo "Building modules."; \
	for dir in $(MODULES); do \
	    $(MAKE) -C "$(abs_top_srcdir)/$$dir" \
		INCLUDE_VIVIANITE_H="$(abs_top_srcdir)/src/include"; \
	done

all-local: modules

DISTCLEANFILES = \
    $(PROJECT_NAME) \
    build-aux/ \
    config.cache \
    config.log \
    config.status \
    configure~ \
    configure.ac~ \
    aclocal.m4 \
    config.guess \
    config.sub \
    depcomp \
    ltmain.sh \
    ar-lib \
    test-driver \
    Makefile.in \
    Makefile
