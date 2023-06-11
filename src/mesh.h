#pragma once

#include "Vector.h"
#include "triangle.h"
#include <vector>

extern "C" {
	#include "texture.h"
}

struct mesh_t
{
	std::vector<Vector3> vertices;
	std::vector<face_t> faces;
	Vector3 rotation;
	Vector3 scale;
	Vector3 translation;
	texture_t mesh_texture;

	int last_triangle_index;
};

typedef struct {
	std::vector<mesh_t> meshes_list;
	int meshes_count;
} mesh_container_t;

void load_obj_file_data(const char* filename, const char* texture_path);
mesh_t* get_mesh_item(int index);
int get_mesh_count();
void free_mesh_container();