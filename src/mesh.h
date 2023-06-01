#pragma once

#include "vector.h"
#include "triangle.h"
#include "texture.h"

typedef struct
{
	vec3_t* vertices;
	face_t* faces;
	vec3_t rotation;
	vec3_t scale;
	vec3_t translation;
	texture_t mesh_texture;

	int last_triangle_index;
} mesh_t;

typedef struct {
	mesh_t* meshes_list;
	int meshes_count;
} mesh_container_t;

void load_obj_file_data(char* filename, char* texture_path);
mesh_t* get_mesh_item(int index);
int get_mesh_count();
void free_mesh_container();