#ifndef SERDES_UTILITY_H
#define SERDES_UTILITY_H

#include <utility>
#include <cstddef>
#include <string>
#include <array>

namespace serdes {

// Utility helper to select valid expressions
template <typename S, typename T>
struct valid_t {
	typedef T type;
};

struct ubiq_lref_count {
	constexpr ubiq_lref_count(size_t _) {}
	template <typename T>
	constexpr operator T ();
};

/* Manual base type type descriptions */

template <typename T>
struct base_type;

template <>
struct base_type<int> {
	static std::string get_type(void) {
		return "int";
	}
	static std::string get_value(const int& v) {
		return std::to_string(v);
	}
};
template <>
struct base_type<float> {
	static std::string get_type(void) {
		return "float";
	}
	static std::string get_value(const float& v) {
		return std::to_string(v);
	}
};
template <>
struct base_type<std::string> {
	static std::string get_type(void) {
		return "std::string";
	}
	static std::string get_value(const std::string& v) {
		// TODO escape the string
		return "\"" + v + "\"";
	}
};
template <>
struct base_type<char> {
	static std::string get_type(void) {
		return "char";
	}
	static std::string get_value(const char& v) {
		char vs[2] = {v, '\0'};
		return "'" + std::string(vs) + "'"; 
	}
};


struct ubiq_lref_type {
	std::string& out;
	constexpr ubiq_lref_type(std::string& out, size_t _): out(out) {}
	
	template <typename T>
	constexpr operator T () {
		out += base_type<T>::get_type() + ", ";
		return T{};
	}
};


template <typename T, size_t...indices>
constexpr auto get_mem_count_internal(int) -> typename valid_t<decltype(T{ubiq_lref_count(indices)...}), size_t>::type {
	return sizeof...(indices);
}

template <typename T, size_t _, size_t...indices>
constexpr size_t get_mem_count_internal(long) {
	return get_mem_count_internal<T, indices...>(0);
}


template <typename T, size_t...indices>
constexpr size_t get_mem_count_impl(std::index_sequence<indices...> _) {
	return get_mem_count_internal<T, indices...>(0);
}

template <typename T>
constexpr size_t get_mem_count(void) {
	return get_mem_count_impl<T>(std::make_index_sequence<sizeof(T)>());
}

template <typename T, size_t...indices> 
std::string get_type_str_impl(std::index_sequence<indices...> _) {
	std::string type_str = "{";
	T{ubiq_lref_type(type_str, indices)...};

	if (type_str.length() != 1) {
		type_str = type_str.substr(0, type_str.length() - 2);
	}

	type_str += "}";
	return type_str;
}

// main entry point for get type definition function 
template <typename T> 
std::string get_type_str(void) {
	// First obtain the number of elements in constexpr
	constexpr size_t num_mem = get_mem_count<T>();

	std::string type_str = get_type_str_impl<T>(std::make_index_sequence<num_mem>());	

	return type_str;
}

template <size_t N>
struct ubiq_lref_offsets {
	std::array<size_t, N> &offsets;
	std::array<size_t, N> &sizes;
	size_t index;

	ubiq_lref_offsets(std::array<size_t, N> &offsets, std::array<size_t, N> &sizes, size_t index): offsets(offsets), sizes(sizes), index(index) {}

	auto alignto(size_t v, size_t a) {
		return ((v + a - 1) / a) * a;
	}
	
	template <typename T>
	operator T () {
		sizes[index] = sizeof(T);
		if (index == 0) return T{};
		offsets[index] = alignto(offsets[index - 1] + sizes[index - 1], alignof(T));
		return T{};
	}
};

template <typename T, size_t...indices> 
auto get_offsets_impl(std::index_sequence<indices...> _) -> std::array<size_t, sizeof...(indices)> {
	
	std::array<size_t, sizeof...(indices)> offsets;
	
	if (sizeof...(indices) == 0) return offsets;

	std::array<size_t, sizeof...(indices)> sizes;

	offsets[0] = 0;
	T{ubiq_lref_offsets<sizeof...(indices)>(offsets, sizes, indices)...};	

	return offsets;	
}


template <typename T, size_t N = get_mem_count<T>()> 
std::array<size_t, N> get_offsets(void) {
	// Get constexpr number of elements and allocate an array to be filled
	return get_offsets_impl<T>(std::make_index_sequence<N>());
}

}

#endif
