#include "mesh.h"
#include "array.h"
#include <stdio.h>
#include <string.h>

static mesh_t mesh_sample = 
{
	.vertices = NULL,
	.faces = NULL,
	.rotation = {.x = 0, .y = 0, .z = 0},
	.scale = {.x = 1.0f, .y = 1.0f, .z = 1.0f},
	.translation = {.x = 0.0f, .y = 0.0f, .z = 0.0f}
};

static mesh_container_t mesh_container =
{
	.meshes_list = NULL,
	.meshes_count = 0
};

vec3_t read_mesh_vertex(char* line)
{
	vec3_t v;
	sscanf_s(line, "v %f %f %f", &v.x, &v.y, &v.z);
	return v;
}

face_t read_mesh_face(char* line, tex2_t* uv_buffer, vec3_t* normal_buffer)
{
	face_t f;

	int uv_a_index, uv_b_index, uv_c_index;
	int normal_a_index, normal_b_index, normal_c_index;

	sscanf_s(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", &f.a, &uv_a_index, &normal_a_index, &f.b, &uv_b_index, &normal_b_index, &f.c, &uv_c_index, &normal_c_index);

	f.a_uv = uv_buffer[uv_a_index - 1];
	f.b_uv = uv_buffer[uv_b_index - 1];
	f.c_uv = uv_buffer[uv_c_index - 1];

	f.a_normal = normal_buffer[normal_a_index - 1];
	f.b_normal = normal_buffer[normal_b_index - 1];
	f.c_normal = normal_buffer[normal_c_index - 1];
	
	f.a -= 1;
	f.b -= 1;
	f.c -= 1;

	f.color = 0xFFFFFFFF;

	return f;
}

tex2_t read_mesh_uv(const char* line)
{
	tex2_t uv;
	sscanf_s(line, "vt %f %f", &uv.u, &uv.v);
	return uv;
}

vec3_t read_mesh_normal(const char* line)
{
	vec3_t normal;
	sscanf_s(line, "vn %f %f %f", &normal.x, &normal.y, &normal.z);
	return normal;
}

void load_obj_file_data(const char* filename, const char* texture_path)
{	
	mesh_t mesh = mesh_sample;

	FILE* fp;
	char line[128];

	fopen_s(&fp, filename, "r");

	if (fp == NULL)
	{
		printf("Error: Could not open file %s\n", filename);
		return;
	}

	tex2_t* uv_buffer = NULL;
	vec3_t* normal_buffer = NULL;
	
	while (fgets(line, sizeof(line), fp) != NULL)
	{
		//printf("%s", line);

		if (strncmp(line, "v ", 2) == 0)
		{
			array_push(mesh.vertices, read_mesh_vertex(line));
		}

		if (strncmp(line, "vt ", 3) == 0)
		{
			array_push(uv_buffer, read_mesh_uv(line));
		}
		
		if (strncmp(line, "vn ", 3) == 0)
		{
			array_push(normal_buffer, read_mesh_normal(line));
		}

		if (strncmp(line, "f ", 2) == 0)
		{
			array_push(mesh.faces, read_mesh_face(line, uv_buffer, normal_buffer));
		}
	}

	array_free(uv_buffer);
	array_free(normal_buffer);

	fclose(fp);

	mesh.mesh_texture = load_png_texture_data(texture_path);

	array_push(mesh_container.meshes_list, mesh);
	mesh_container.meshes_count += 1;
}

mesh_t* get_mesh_item(int index)
{
	if (index >= mesh_container.meshes_count) return NULL;
	return &mesh_container.meshes_list[index];
}

int get_mesh_count()
{
	return mesh_container.meshes_count;
}

static void free_mesh(mesh_t* mesh)
{
	array_free(mesh->faces);
	array_free(mesh->vertices);
	free_texture(&(mesh->mesh_texture));
}

void free_mesh_container()
{
	for (int i = 0; i < mesh_container.meshes_count; i++)
	{
		free_mesh(get_mesh_item(i));
	}

	array_free(mesh_container.meshes_list);
}
