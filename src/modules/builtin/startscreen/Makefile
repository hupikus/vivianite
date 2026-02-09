NAME=module.so
CXX ?= g++


CXX_SRC = startscreen.cpp
CXX_INCLUDE = -I../../../../include -I/usr/include/GL/
PKG_LIBS = $(shell pkg-config --cflags --libs freetype2)
all:
	$(CXX) -fPIC -shared $(CXX_INCLUDE) $(CXX_LINK) $(PKG_LIBS) $(CXX_SRC) -o $(NAME)

clean:
	rm $(NAME)
