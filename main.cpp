#include "serialize.h"
#include <cassert>
#include <iostream>

struct foo {
	int x;
	float y;
	char a;
	char b;
	std::string z;
};


int main(int argc, char* argv[]) {

	std::cout << serdes::get_mem_count<foo>() << std::endl;

	std::cout << serdes::get_type_str<foo>() << std::endl;

	auto offsets = serdes::get_offsets<foo>();

	for (size_t s = 0; s < offsets.size(); s++) {
		std::cout << offsets[s];
		if (s != offsets.size() - 1) {
			std::cout << ", ";
		}
	}
	std::cout << std::endl;

	foo obj = {.x = 42, .y = 6.9, .a = 'a', .b = 'B', .z = "Hello"};

	std::cout << serdes::serialize(obj) << std::endl;

		
	return 0;
}
