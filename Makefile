all: build/main
build/main: main.cpp $(wildcard *.h)
	mkdir -p $(@D)
	g++ -O3 -I include -std=c++14 $< -o $@


clean: 
	rm -rf build/
