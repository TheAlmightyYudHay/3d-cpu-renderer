#include "mesh.h"
#include "array.h"
#include <stdio.h>
#include <string.h>

mesh_t mesh = 
{
	.vertices = NULL,
	.faces = NULL,
	.rotation = {.x = 0.63f, .y = 0.43f, .z = 0}
};

vec3_t cube_vertices[N_CUBE_VERTICES] = {
	{.x = -1, .y = -1, .z = -1 }, //1
	{.x = -1, .y = 1, .z = -1 }, //2
	{.x = 1, .y = 1, .z = -1 }, //3
	{.x = 1, .y = -1, .z = -1 }, //4
	{.x = 1, .y = 1, .z = 1 }, //5
	{.x = 1, .y = -1, .z = 1 }, //6
	{.x = -1, .y = 1, .z = 1 }, //7
	{.x = -1, .y = -1, .z = 1 }  //8
};

face_t cube_faces[N_CUBE_FACES] = {
	// front
	{.a = 1, .b = 2, .c = 3},
	{.a = 1, .b = 3, .c = 4},
	// right
	{.a = 4, .b = 3, .c = 5},
	{.a = 4, .b = 5, .c = 6},
	// back
	{.a = 6, .b = 5, .c = 7},
	{.a = 6, .b = 7, .c = 8},
	// left
	{.a = 8, .b = 7, .c = 2},
	{.a = 8, .b = 2, .c = 1},
	// top
	{.a = 2, .b = 7, .c = 5},
	{.a = 2, .b = 5, .c = 3},
	// bottom
	{.a = 6, .b = 8, .c = 1},
	{.a = 6, .b = 1, .c = 4}
};

void load_cube_mesh_data(void)
{
	for (int i = 0; i < N_CUBE_VERTICES; i++)
	{
		array_push(mesh.vertices, cube_vertices[i]);
	}

	for (int i = 0; i < N_CUBE_FACES; i++)
	{
		array_push(mesh.faces, cube_faces[i]);
	}
}

vec3_t read_mesh_vertex(char* line)
{
	vec3_t v;
	sscanf_s(line, "v %f %f %f", &v.x, &v.y, &v.z);
	return v;
}

face_t read_mesh_face(char* line)
{
	face_t f;
	sscanf_s(line, "f %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &f.a, &f.b, &f.c);

	f.a -= 1;
	f.b -= 1;
	f.c -= 1;

	return f;
}

void load_obj_file_data(char* filename)
{
	FILE* fp;
	char line[128];

	fopen_s(&fp, filename, "r");

	if (fp == NULL)
	{
		printf("Error: Could not open file %s\n", filename);
		return;
	}
	
	while (fgets(line, sizeof(line), fp) != NULL)
	{
		//printf("%s", line);

		if (strncmp(line, "v ", 2) == 0)
		{
			array_push(mesh.vertices, read_mesh_vertex(line));
		}

		if (strncmp(line, "f ", 2) == 0)
		{
			array_push(mesh.faces, read_mesh_face(line));
		}
	}

	fclose(fp);
}
