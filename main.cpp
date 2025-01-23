#include "serdes/serialize.h"
#include <cassert>
#include <iostream>

struct foo {
	int x;
	float y;
	char a;
	char b;
	std::string z;
};

struct bar {

};

int main(int argc, char* argv[]) {

	std::cout << serdes::get_mem_count<foo>() << std::endl;

	std::cout << serdes::get_type_str<foo>() << std::endl;

	auto offsets = serdes::get_offsets<foo>();

	serdes::get_offsets<bar>();

	for (auto offset: offsets) {
		std::cout << offset << ", ";
	}
	std::cout << std::endl;

	foo obj = {.x = 42, .y = 6.9, .a = 'a', .b = 'B', .z = "Hello"};

	std::cout << serdes::serialize(obj) << std::endl;

		
	return 0;
}
