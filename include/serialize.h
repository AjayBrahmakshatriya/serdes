#ifndef SERIALIZE_H
#define SERIALIZE_H

#include "utility.h"

namespace serdes {

struct ubiq_lref_serialize {
	const void* obj;
	std::string& out;
	size_t offset;
	
	constexpr ubiq_lref_serialize(const void* obj, std::string& out, size_t offset, size_t _): out(out), obj(obj), offset(offset) {}
	
	template <typename T>
	constexpr operator T () {
		const T* v = (const T*) ((const char*) obj + offset);
		out += base_type<T>::get_value(*v) + ", ";
		return T{};
	}
};
template <typename T, typename OT, size_t...indices>
std::string serialize_impl (const T& obj, std::index_sequence<indices...>_, const OT& offsets) {
	std::string output;
	T{ubiq_lref_serialize((const void*)&obj, output, offsets[indices], indices)...};
	if (output.length() != 0) {
		output = output.substr(0, output.length() - 2);
	}
	return output;
}

// main entry point for the serialize function
template <typename T>
std::string serialize (const T& obj) {
	// First obtain the number of elements in constexpr	
	constexpr size_t num_mem = get_mem_count<T>();
	// Now obtain the offsets of each element
	auto offsets = get_offsets<T>();

	return serialize_impl(obj, std::make_index_sequence<num_mem>(), offsets);
}




}


#endif
