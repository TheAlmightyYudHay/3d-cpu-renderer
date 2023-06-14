#include "mesh.h"
#include <stdio.h>
#include <string.h>

Vector3 read_mesh_vertex(char* line)
{
	float x, y, z;
	sscanf_s(line, "v %f %f %f", &x, &y, &z);
	return {x, y, z};
}

face_t read_mesh_face(char* line, const std::vector<tex2_t>& uv_buffer, const std::vector<Vector3>& normal_buffer)
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

Vector3 read_mesh_normal(const char* line)
{
	float x, y, z;
	sscanf_s(line, "vn %f %f %f", &x, &y, &z);
	return {x, y, z};
}

void MeshContainer::LoadMeshData(const char* filename, const char* texture_path)
{
	Mesh mesh{};

	FILE* fp;
	char line[128];

	fopen_s(&fp, filename, "r");

	if (fp == NULL)
	{
		printf("Error: Could not open file %s\n", filename);
		return;
	}

	std::vector<tex2_t> uv_buffer;
	std::vector<Vector3> normal_buffer;

	while (fgets(line, sizeof(line), fp) != NULL)
	{
		//printf("%s", line);

		if (strncmp(line, "v ", 2) == 0)
		{
			mesh.AddVertex(read_mesh_vertex(line));
		}

		if (strncmp(line, "vt ", 3) == 0)
		{
			uv_buffer.push_back(read_mesh_uv(line));
		}

		if (strncmp(line, "vn ", 3) == 0)
		{
			normal_buffer.push_back(read_mesh_normal(line));
		}

		if (strncmp(line, "f ", 2) == 0)
		{
			mesh.AddFace(read_mesh_face(line, uv_buffer, normal_buffer));
		}
	}

	fclose(fp);

	mesh.SetTexture(load_png_texture_data(texture_path));

	mMeshes.push_back(mesh);
	mMeshesCount += 1;
}

void MeshContainer::FreeMeshContainer()
{
	for (Mesh& mesh : mMeshes)
	{
		mesh.FreeTexture();
	}

	mMeshes.clear();
	mMeshesCount = 0;
}
