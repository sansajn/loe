/* v20111001 */
#pragma once

#include <cstddef>
#include <tr1/tuple>

namespace lua { 
	namespace obj {

typedef std::size_t size_type;
typedef std::ptrdiff_t index_type;
typedef std::tr1::tuple<index_type, index_type> index_2_tuple_type;
typedef std::tr1::tuple<index_type, index_type, index_type> index_3_tuple_type;

	}  // obj
}  // lua

