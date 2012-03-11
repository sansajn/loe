/* v20110928 */
#include <iostream>
#include <loe/obj.hpp>
namespace obj = lua::obj;


void vertex_callback(float x, float y, float z)
{
	std::cout << "v " << x << " " << y << " " << z << "\n";
}

void face_callback(obj::index_type i, obj::index_type j, obj::index_type k,
	obj::index_type m)
{
	std::cout << "f " << i << " " << j << " " << k << " " << m << "\n";
}

int main(int argc, char* argv[])
{
	obj::obj_parser obj_parser;
	obj_parser.geometric_vertex_callback = vertex_callback;
	obj_parser.face4_vertices_callback = face_callback;
	obj_parser.parse("cube.obj");
}
