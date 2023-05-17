#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "triangle.h"
#include "array.h"

triangle_t* triangles_to_render = NULL;

vec3_t camera_positon = { .x = 0, .y = 0, .z = 2 };
float fov_factor = 640.0;

bool is_running = false;
uint32_t previous_frame_time = 0;

void setup(void)
{
	color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * window_width * window_height);

	color_buffer_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		window_width,
		window_height
	);

	load_obj_file_data("./assets/cube.obj");
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

	mesh.rotation.x += 0.025f;
	mesh.rotation.y += 0.025f;
	mesh.rotation.z += 0.025f;

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
			vec3_t transformed_vertex = face_vertices[vIndex];

			transformed_vertex = vec3_rotate_x(transformed_vertex, mesh.rotation.x);
			transformed_vertex = vec3_rotate_y(transformed_vertex, mesh.rotation.y);
			transformed_vertex = vec3_rotate_z(transformed_vertex, mesh.rotation.z);

			transformed_vertex.z += 5;


			face_vertices[vIndex] = transformed_vertex;
		}
		
		if (is_should_be_culled(face_vertices[0], face_vertices[1], face_vertices[2]))
		{
			continue;
		}

		triangle_t projected_triangle = { .points = {0} };

		for (int vIndex = 0; vIndex < 3; vIndex++)
		{
			vec2_t projected_point = project(face_vertices[vIndex]);
			//Apply projection translate
			projected_point.x += (window_width / 2);
			projected_point.y += (window_height / 2);

			projected_triangle.points[vIndex] = projected_point;
		}
		
		array_push(triangles_to_render, projected_triangle);
		
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

		fill_triangle(triangle, 0xFFFFFFFF);
		draw_triangle(
			triangle.points[0].x, triangle.points[0].y,
			triangle.points[1].x, triangle.points[1].y,
			triangle.points[2].x, triangle.points[2].y,
			0xFF000000
		);
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