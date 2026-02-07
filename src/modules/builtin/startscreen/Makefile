NAME=module.so
CXX ?= g++


CXX_SRC = startscreen.cpp
CXX_INCLUDE = -I../../../../include

all:
	$(CXX) -fPIC -shared $(CXX_INCLUDE) $(CXX_SRC) -o $(NAME)

clean:
	rm $(NAME)
