all: build/main
build/main: main.cpp $(wildcard *.h)
	mkdir -p $(@D)
	g++ -O3 -std=c++14 $< -o $@
