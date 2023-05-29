#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "triangle.h"
#include "array.h"
#include "matrix.h"
#include "light.h"
#include "texture.h"
#include "upng.h"
#include "camera.h"

#define MAX_TRIANGLES_PER_MESH 10000
triangle_t triangles_to_render[MAX_TRIANGLES_PER_MESH];
int num_triangles_to_render = 0;

int rendering_mode = 0;
mat4_t projection_matrix = { 0 };

bool is_running = false;
uint32_t previous_frame_time = 0;
float delta_time;

void setup(void)
{
	rendering_mode = textured_mask | backface_culling_mask;

	color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * window_width * window_height);
	z_buffer = (float*)malloc(sizeof(float) * window_width * window_height);

	color_buffer_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_RGBA32,
		SDL_TEXTUREACCESS_STREAMING,
		window_width,
		window_height
	);


	vec3_normalize(&light.direction);

	float fov = M_PI * 60.0 / 180.0;
	float aspect = window_height / (float)window_width;
	float zNear = 0.1;
	float zFar = 100.0;

	projection_matrix = mat4_make_perspective(fov, aspect, zNear, zFar);

	// Load hardcoded texture data
	load_png_texture_data("./assets/f22.png");

	//load_cube_mesh_data();
	load_obj_file_data("./assets/f22.obj");
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
			if (event.key.keysym.sym == SDLK_5)
				rendering_mode = textured_mask | (rendering_mode & backface_culling_mask & backface_culling_mask);
			if (event.key.keysym.sym == SDLK_6)
				rendering_mode = textured_mask | wireframe_mask | (rendering_mode & backface_culling_mask & backface_culling_mask);
			if (event.key.keysym.sym == SDLK_c)
				rendering_mode ^= backface_culling_mask;
			if (event.key.keysym.sym == SDLK_f)
				rendering_mode ^= lighting_mask;
			if (event.key.keysym.sym == SDLK_UP)
				camera.position.y += 3.0 * delta_time;
			if (event.key.keysym.sym == SDLK_DOWN)
				camera.position.y -= 3.0 * delta_time;
			if (event.key.keysym.sym == SDLK_a)
				camera.yaw_angle -= 1.0 * delta_time;
			if (event.key.keysym.sym == SDLK_d)
				camera.yaw_angle += 1.0 * delta_time;
			if (event.key.keysym.sym == SDLK_w)
			{
				camera.forward_velocity = vec3_mul(camera.direction, 5.0 * delta_time);
				camera.position = vec3_add(camera.position, camera.forward_velocity);
			}
			if (event.key.keysym.sym == SDLK_s)
			{
				camera.forward_velocity = vec3_mul(camera.direction, 5.0 * delta_time);
				camera.position = vec3_sub(camera.position, camera.forward_velocity);
			}
			break;
		default:
			break;
	}
}

vec3_t calculate_face_normal(vec3_t p1, vec3_t p2, vec3_t p3)
{
	vec3_t a = vec3_sub(p2, p1);
	vec3_t b = vec3_sub(p3, p1);
	vec3_t normal = vec3_cross(a, b);
	vec3_normalize(&normal);
	return normal;
}

bool is_should_be_culled(vec3_t pointOn, vec3_t normal)
{
	if (!(rendering_mode & backface_culling_mask))
		return false;

	vec3_t origin = { 0, 0, 0 };
	vec3_t camera_ray = vec3_sub(origin, pointOn);
	return vec3_dot(normal, camera_ray) <= 0.0;
}

void update(void)
{
	uint32_t time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);

	if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
	{
		SDL_Delay(time_to_wait);
	}

	delta_time = (SDL_GetTicks() - previous_frame_time) / 1000.0;

	previous_frame_time = SDL_GetTicks();

	num_triangles_to_render = 0;

	//mesh.rotation.x += 1.0f * delta_time;
	//mesh.rotation.y += 3.141592f * delta_time;
	//mesh.rotation.z += 1.0f * delta_time;

	/*mesh.scale.x = 4;
	mesh.scale.y = 4;
	mesh.scale.z = 4;*/

	//mesh.translation.x = 5;
	//mesh.translation.y += 0.005f * delta_time;
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

	mat4_t normal_matrix = mat4_identity();
	normal_matrix = mat4_mul_mat4(rotate_z_matrix, normal_matrix);
	normal_matrix = mat4_mul_mat4(rotate_y_matrix, normal_matrix);
	normal_matrix = mat4_mul_mat4(rotate_x_matrix, normal_matrix);

	//camera.position.x += 1 * delta_time;
	//camera.position.y += 1 * delta_time;
	vec3_t up_direction = { 0, 1, 0 };
	
	vec3_t target = { 0, 0, 1 };
	mat4_t camera_yaw_rotation = mat4_make_rotation_y(camera.yaw_angle);
	camera.direction = vec3_from_vec4(mat4_mul_vec4(camera_yaw_rotation, vec4_from_vec3(target)));

	target = vec3_add(camera.position, camera.direction);

	mat4_t view_matrix = mat4_look_at(camera.position, target, up_direction);
	//world_matrix = mat4_mul_mat4(view_matrix, world_matrix);
	//normal_matrix = mat4_mul_mat4(view_matrix, normal_matrix);

	int num_faces = array_length(mesh.faces);
	for (int i = 0; i < num_faces; i++)
	{
		face_t mesh_face = mesh.faces[i];
		vec3_t face_vertices[3] = {
			mesh.vertices[mesh_face.a],
			mesh.vertices[mesh_face.b],
			mesh.vertices[mesh_face.c],
		};

		vec3_t face_normals[3] = {
			mesh_face.a_normal,
			mesh_face.b_normal,
			mesh_face.c_normal
		};


		for (int vIndex = 0; vIndex < 3; vIndex++)
		{
			vec4_t transformed_vertex = vec4_from_vec3(face_vertices[vIndex]);
			vec4_t transformed_normal = {
				.x = face_normals[vIndex].x,
				.y = face_normals[vIndex].y,
				.z = face_normals[vIndex].z,
				.w = 0
			};

			transformed_vertex = mat4_mul_vec4(world_matrix, transformed_vertex);
			transformed_vertex = mat4_mul_vec4(view_matrix, transformed_vertex);
			
			transformed_normal = mat4_mul_vec4(normal_matrix, transformed_normal);
			transformed_normal = mat4_mul_vec4(view_matrix, transformed_normal);

			face_vertices[vIndex] = vec3_from_vec4(transformed_vertex);
			face_normals[vIndex] = vec3_from_vec4(transformed_normal);
		}

		mesh_face.normal = calculate_face_normal(face_vertices[0], face_vertices[1], face_vertices[2]);
		
		if (is_should_be_culled(face_vertices[0], mesh_face.normal))
		{
			continue;
		}

		light.view = vec3_from_vec4(mat4_mul_vec4(view_matrix, (vec4_t){
			.x = light.direction.x,
			.y = light.direction.y,
			.z = light.direction.z,
			.w = 0
		}));

		float light_intencity_factor = vec3_dot(mesh_face.normal, vec3_negative(light.view));
		uint32_t triangle_color = light_apply_intensity(mesh_face.color, light_intencity_factor);

		triangle_t projected_triangle = {
			.points = {0},
			.texcoords = {
				mesh_face.a_uv,
				mesh_face.b_uv,
				mesh_face.c_uv,
			},
			.normals = {
				face_normals[0],
				face_normals[1],
				face_normals[2],
			},
			.color = triangle_color
		};

		for (int vIndex = 0; vIndex < 3; vIndex++)
		{
			vec4_t projected_point = mat4_mul_vec4_project(projection_matrix, vec4_from_vec3(face_vertices[vIndex]));
			
			//Scale into the view
			projected_point.x *= (window_width / 4.0);
			projected_point.y *= (window_height / 4.0);

			//Apply projection translate
			projected_point.x += (window_width / 2.0);
			projected_point.y += (window_height / 2.0);


			projected_triangle.points[vIndex] = projected_point;
		}

		if (num_triangles_to_render < MAX_TRIANGLES_PER_MESH)
		{
			triangles_to_render[num_triangles_to_render] = projected_triangle;
			num_triangles_to_render += 1;
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
	draw_grid(0xFF333333);

	for (int i = 0; i < num_triangles_to_render; i++)
	{
		triangle_t triangle = triangles_to_render[i];

		if (rendering_mode & filled_mask)
		{
			fill_triangle(triangle, triangle.color);
		}

		if (rendering_mode & textured_mask)
		{
			draw_textured_triangle(triangle, mesh_texture, rendering_mode & lighting_mask);
		}

		if (rendering_mode & vertices_mask)
		{
			draw_vertices(triangle, 0xFFFF0000);
		}
	}
	if (rendering_mode & wireframe_mask)
	{
		for (int i = 0; i < num_triangles_to_render; i++)
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

	render_color_buffer();
	
	clear_z_buffer();
	clear_color_buffer(0xFF000000);

	SDL_RenderPresent(renderer);
}

void free_resources(void)
{
	free(color_buffer);
	free(z_buffer);
	upng_free(png_texture);
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