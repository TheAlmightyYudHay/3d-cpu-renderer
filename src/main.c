#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "triangle.h"
#include "array.h"
#include "matrix.h"

triangle_t* triangles_to_render = NULL;

vec3_t camera_positon = { .x = 0, .y = 0, .z = 0 };
float fov_factor = 640.0;

int rendering_mode = 0;


bool is_running = false;
uint32_t previous_frame_time = 0;

void setup(void)
{
	rendering_mode = wireframe_mask | backface_culling_mask;

	color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * window_width * window_height);

	color_buffer_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		window_width,
		window_height
	);

	load_cube_mesh_data();
	//load_obj_file_data("./assets/cube.obj");
}

void process_input(void)
{
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type)
	{
		case SDL_QUIT:
			is_running = false;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
				is_running = false;
			if (event.key.keysym.sym == SDLK_1)
				rendering_mode = wireframe_mask | vertices_mask | (rendering_mode & backface_culling_mask & backface_culling_mask);
			if (event.key.keysym.sym == SDLK_2)
				rendering_mode = wireframe_mask | (rendering_mode & backface_culling_mask & backface_culling_mask);
			if (event.key.keysym.sym == SDLK_3)
				rendering_mode = filled_mask | (rendering_mode & backface_culling_mask & backface_culling_mask);
			if (event.key.keysym.sym == SDLK_4)
				rendering_mode = filled_mask | wireframe_mask | (rendering_mode & backface_culling_mask & backface_culling_mask);
			if (event.key.keysym.sym == SDLK_c)
				rendering_mode |= backface_culling_mask;
			if (event.key.keysym.sym == SDLK_d)
				rendering_mode &= ~backface_culling_mask;
			break;
		default:
			break;
	}
}

vec2_t project(vec3_t point)
{
	float zDivizor = 1 / (point.z - camera_positon.z);
	vec2_t projected_point = {
		.x = fov_factor * point.x * zDivizor,
		.y = fov_factor * point.y * zDivizor
	};
	return projected_point;
}

bool is_should_be_culled(vec3_t p1, vec3_t p2, vec3_t p3)
{
	if (!(rendering_mode & backface_culling_mask))
		return false;

	vec3_t a = vec3_sub(p2, p1);
	vec3_t b = vec3_sub(p3, p1);
	vec3_t normal = vec3_cross(a, b);
	vec3_t camera_ray = vec3_sub(camera_positon, p1);
	return vec3_dot(normal, camera_ray) <= 0.0;
}

void update(void)
{
	uint32_t time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);

	if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
	{
		SDL_Delay(time_to_wait);
	}

	previous_frame_time = SDL_GetTicks();

	triangles_to_render = NULL;

	//mesh.rotation.x += 0.01f;
	//mesh.rotation.y += 0.01f;
	mesh.rotation.z += 0.01f;

	mesh.scale.x += 0.002f;
	//mesh.scale.y += 0.002f;

	//mesh.translation.x += 0.01f;
	//mesh.translation.y += 0.01f;
	mesh.translation.z = 5;

	mat4_t scale_matrix = mat4_make_scale(mesh.scale.x, mesh.scale.y, mesh.scale.z);
	mat4_t translation_matrix = mat4_make_translation(mesh.translation.x, mesh.translation.y, mesh.translation.z);
	mat4_t rotate_x_matrix = mat4_make_rotation_x(mesh.rotation.x);
	mat4_t rotate_y_matrix = mat4_make_rotation_y(mesh.rotation.y);
	mat4_t rotate_z_matrix = mat4_make_rotation_z(mesh.rotation.z);
	
	mat4_t world_matrix = mat4_identity();
	world_matrix = mat4_mul_mat4(scale_matrix, world_matrix);
	world_matrix = mat4_mul_mat4(rotate_z_matrix, world_matrix);
	world_matrix = mat4_mul_mat4(rotate_y_matrix, world_matrix);
	world_matrix = mat4_mul_mat4(rotate_x_matrix, world_matrix);
	world_matrix = mat4_mul_mat4(translation_matrix, world_matrix);

	int num_faces = array_length(mesh.faces);
	for (int i = 0; i < num_faces; i++)
	{
		face_t mesh_face = mesh.faces[i];
		vec3_t face_vertices[3] = {
			mesh.vertices[mesh_face.a],
			mesh.vertices[mesh_face.b],
			mesh.vertices[mesh_face.c],
		};


		for (int vIndex = 0; vIndex < 3; vIndex++)
		{
			vec4_t transformed_vertex = vec4_from_vec3(face_vertices[vIndex]);

			transformed_vertex = mat4_mul_vec4(world_matrix, transformed_vertex);

			face_vertices[vIndex] = vec3_from_vec4(transformed_vertex);
		}
		
		if (is_should_be_culled(face_vertices[0], face_vertices[1], face_vertices[2]))
		{
			continue;
		}

		triangle_t projected_triangle = { .points = {0}, .color = mesh_face.color, .avg_depth = 0.0f };

		for (int vIndex = 0; vIndex < 3; vIndex++)
		{
			vec2_t projected_point = project(face_vertices[vIndex]);
			//Apply projection translate
			projected_point.x += (window_width / 2);
			projected_point.y += (window_height / 2);

			projected_triangle.points[vIndex] = projected_point;
			projected_triangle.avg_depth += face_vertices[vIndex].z;
		}

		array_push(triangles_to_render, projected_triangle);
		
	}

	int num_triangles = array_length(triangles_to_render);
	for (int i = 0; i < num_triangles - 1; i++)
	{
		for (int j = i + 1; j < num_triangles; j++)
		{
			if (triangles_to_render[i].avg_depth < triangles_to_render[j].avg_depth)
			{
				triangle_t temp = triangles_to_render[i];
				triangles_to_render[i] = triangles_to_render[j];
				triangles_to_render[j] = temp;
			}
		}
	}
}

void draw_vertices(triangle_t triangle, uint32_t color)
{
	for (int i = 0; i < 3; i++)
	{
		draw_rect(triangle.points[i].x - 2, triangle.points[i].y - 2, 4, 4, color);
	}
}

void render(void)
{
	clear_color_buffer(0xFF000000);
	draw_grid(0xFF333333);
	
	int num_triangles = array_length(triangles_to_render);

	for (int i = 0; i < num_triangles; i++)
	{
		triangle_t triangle = triangles_to_render[i];

		if (rendering_mode & filled_mask)
		{
			fill_triangle(triangle, triangle.color);
		}

		if (rendering_mode & vertices_mask)
		{
			draw_vertices(triangle, 0xFFFF0000);
		}
	}
	if (rendering_mode & wireframe_mask)
	{
		for (int i = 0; i < num_triangles; i++)
		{
			triangle_t triangle = triangles_to_render[i];

			draw_wireframe(
				triangle.points[0].x, triangle.points[0].y,
				triangle.points[1].x, triangle.points[1].y,
				triangle.points[2].x, triangle.points[2].y,
				0xFF00FFFF
			);

		}
	}

	array_free(triangles_to_render);

	render_color_buffer();

	SDL_RenderPresent(renderer);
}

void free_resources(void)
{
	free(color_buffer);
	array_free(mesh.faces);
	array_free(mesh.vertices);
}

int main(int argc, char* args[])
{
	is_running = initialize_window();

	setup();

	while (is_running)
	{
		process_input();
		update();
		render();
	}

	destroy_window();
	free_resources();

	return 0;
}