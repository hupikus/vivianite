NAME=module.so
CXX ?= g++


CXX_SRC = buffer.cpp
CXX_INCLUDE = -I../../../../include -I/usr/include/GL/
all:
	$(CXX) -fPIC -shared $(CXX_INCLUDE) $(CXX_SRC) -o $(NAME)

clean:
	rm $(NAME)
