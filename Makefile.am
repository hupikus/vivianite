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

SRC_COUNT = $(words $(CXX_SRC))


CC_OBJS = $(patsubst %.c,$(OBJDIR)/%.o,$(CC_SRC))
CXX_OBJS = $(patsubst %.cpp,$(OBJDIR)/%.o,$(CXX_SRC))

vivianite_SOURCES = $(CXX_SRC)
override vivianite_OBJECTS = $(CXX_OBJS)

COMPILED_FILES =
$(OBJDIR)/%.opp: $(srcdir)/%.cpp
	@$(eval COMPILED_FILES += $<)
	@$(eval COMPILED_FILES_COUNT = $(words $(COMPILED_FILES)))
	@printf "[$(COMPILED_FILES_COUNT)/$(FILE_COUNT)] "
	
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(AM_CPPFLAGS) $(CXXFLAGS) $(AM_CXXFLAGS) \
	    -MD -MP -MF $@.d -MT $@ -c $< -o $@




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
    Makefile.in
