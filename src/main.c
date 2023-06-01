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
#include "clipping.h"

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
	rendering_mode = wireframe_mask | backface_culling_mask;


	void normalize_light_directon();

	float aspecty = get_window_height() / (float)get_window_width();
	float aspectx = get_window_width() / (float)get_window_height();
	float fovy = M_PI * 60.0 / 180.0;
	float fovx = 2 * atan(aspectx * tan(fovy / 2.0));
	float zNear = 0.1;
	float zFar = 100.0;

	init_frustum_planes(fovy, fovx, zNear, zFar);

	projection_matrix = mat4_make_perspective(fovy, aspecty, zNear, zFar);

	// Load hardcoded texture data
	//load_png_texture_data("./assets/drone.png");

	//load_cube_mesh_data();
	load_obj_file_data("./assets/cube.obj", "./assets/pikuma.png");
	load_obj_file_data("./assets/drone.obj", "./assets/drone.png");
	load_obj_file_data("./assets/cube.obj", "./assets/cube.png");
}

void process_input(void)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			is_running = false;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				is_running = false;
				break;
			}
			if (event.key.keysym.sym == SDLK_1)
			{
				rendering_mode = wireframe_mask | vertices_mask | (rendering_mode & backface_culling_mask & backface_culling_mask);
				break;
			}
			if (event.key.keysym.sym == SDLK_2)
			{
				rendering_mode = wireframe_mask | (rendering_mode & backface_culling_mask & backface_culling_mask);
				break;
			}
			if (event.key.keysym.sym == SDLK_3)
			{
				rendering_mode = filled_mask | (rendering_mode & backface_culling_mask & backface_culling_mask);
				break;
			}
			if (event.key.keysym.sym == SDLK_4)
			{
				rendering_mode = filled_mask | wireframe_mask | (rendering_mode & backface_culling_mask & backface_culling_mask);
				break;
			}
			if (event.key.keysym.sym == SDLK_5)
			{
				rendering_mode = textured_mask | (rendering_mode & backface_culling_mask & backface_culling_mask);
				break;
			}
			if (event.key.keysym.sym == SDLK_6)
			{
				rendering_mode = textured_mask | wireframe_mask | (rendering_mode & backface_culling_mask & backface_culling_mask);
				break;
			}
			if (event.key.keysym.sym == SDLK_c)
			{
				rendering_mode ^= backface_culling_mask;
				break;
			}
			if (event.key.keysym.sym == SDLK_f)
			{
				rendering_mode ^= lighting_mask;
				break;
			}
			if (event.key.keysym.sym == SDLK_UP)
			{
				change_camera_position((vec3_t) {0, 5 * delta_time, 0});
				break;
			}
			if (event.key.keysym.sym == SDLK_DOWN)
			{
				change_camera_position((vec3_t) { 0, -5 * delta_time, 0 });
				break;
			}
			if (event.key.keysym.sym == SDLK_a)
			{
				change_camera_yaw(-3 * delta_time);
				break;
			}
			if (event.key.keysym.sym == SDLK_d)
			{
				change_camera_yaw(3 * delta_time);
				break;
			}
			if (event.key.keysym.sym == SDLK_q)
			{
				change_camera_pitch(-3 * delta_time);
				break;
			}
			if (event.key.keysym.sym == SDLK_e)
			{
				change_camera_pitch(3 * delta_time);
				break;
			}
			if (event.key.keysym.sym == SDLK_w)
			{
				vec3_t offset = vec3_mul(get_camera_directon(), 5.0 * delta_time);
				change_camera_position(offset);
				break;
			}
			if (event.key.keysym.sym == SDLK_s)
			{
				vec3_t offset = vec3_mul(get_camera_directon(), -5.0 * delta_time);
				change_camera_position(offset);
				break;
			}
			break;
		default:
			break;
		}
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

triangle_t create_triangle(polygon_t* polygon, int index0, int index1, int index2)
{

	triangle_t triangle = {
			.points = {
				vec4_from_vec3(polygon->vertices[index0]),
				vec4_from_vec3(polygon->vertices[index1]),
				vec4_from_vec3(polygon->vertices[index2])
			},
			.texcoords = {
				polygon->texcoords[index0],
				polygon->texcoords[index1],
				polygon->texcoords[index2]
			},
			.normals = {
				polygon->normals[index0],
				polygon->normals[index1],
				polygon->normals[index2]
			},
			.color = 0xFFFFFFFF
	};

	if (rendering_mode & lighting_mask)
	{
		vec3_t face_normal = calculate_face_normal(polygon->vertices[index0], polygon->vertices[index1], polygon->vertices[index2]);
		float light_intencity_factor = vec3_dot(face_normal, vec3_negative(get_light_view()));
		triangle.color = light_apply_intensity(triangle.color, light_intencity_factor);
	}

	return triangle;
}

void project_triangle(triangle_t* triangle)
{
	for (int vIndex = 0; vIndex < 3; vIndex++)
	{
		vec4_t projected_point = mat4_mul_vec4_project(projection_matrix, triangle->points[vIndex]);

		////Scale into the view
		projected_point.x = (projected_point.x / 2.0 + 0.5) * (get_window_width() - 1);
		projected_point.y = (projected_point.y / 2.0 + 0.5) * (get_window_height() - 1);


		triangle->points[vIndex] = projected_point;
	}
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

	//////////////////////////////////////////////////////////////////////
	// SETUP CAMERA AND LIGHT
	//////////////////////////////////////////////////////////////////////
	vec3_t up_direction = { 0, 1, 0 };
	vec3_t target = { 0, 0, 100 };

	mat4_t camera_yaw_rotation = mat4_make_rotation_y(get_camera_yaw());
	mat4_t camera_pitch_rotation = mat4_make_rotation_x(get_camera_pitch());
	mat4_t rotation_matrix = mat4_mul_mat4(camera_yaw_rotation, camera_pitch_rotation);

	target = vec3_from_vec4(mat4_mul_vec4(rotation_matrix, vec4_from_vec3(target)));

	set_camera_direction(vec3_normalized(target));

	mat4_t view_matrix = mat4_look_at(get_camera_position(), target, up_direction);

	// Light in view space
	vec3_t new_view_direction = vec3_from_vec4(mat4_mul_vec4(view_matrix, (vec4_t) {
		.x = get_light_direction().x,
		.y = get_light_direction().y,
		.z = get_light_direction().z,
		.w = 0
	}));

	set_view_light(vec3_normalized(new_view_direction));
	//////////////////////////////////////////////////////////////////////

	for (int i = 0; i < get_mesh_count(); i++)
	{
		mesh_t* mesh = get_mesh_item(i);

		mesh->last_triangle_index = 0;

		mesh->rotation.x += 1.0f * delta_time;
		mesh->rotation.y += 1.141592f * delta_time;
		mesh->rotation.z += 1.0f * delta_time;

		/*mesh->rotation.y = M_PI / 4;
		mesh->rotation.x = M_PI / 4;*/

		/*mesh->scale.x += 1 * delta_time;
		mesh->scale.y += 1 * delta_time;
		mesh->scale.z += 1 * delta_time;*/

		//mesh->translation.x = 5;
		//mesh->translation.y -= 0.3 * delta_time;
		mesh->translation.x = 3 * i - 3;
		mesh->translation.z = 3 * i + 3;

		mat4_t scale_matrix = mat4_make_scale(mesh->scale.x, mesh->scale.y, mesh->scale.z);
		mat4_t translation_matrix = mat4_make_translation(mesh->translation.x, mesh->translation.y, mesh->translation.z);
		mat4_t rotate_x_matrix = mat4_make_rotation_x(mesh->rotation.x);
		mat4_t rotate_y_matrix = mat4_make_rotation_y(mesh->rotation.y);
		mat4_t rotate_z_matrix = mat4_make_rotation_z(mesh->rotation.z);

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

		int num_faces = array_length(mesh->faces);
		for (int i = 0; i < num_faces; i++)
		{
			face_t mesh_face = mesh->faces[i];
			vec3_t face_vertices[3] = {
				mesh->vertices[mesh_face.a],
				mesh->vertices[mesh_face.b],
				mesh->vertices[mesh_face.c],
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


			// Clipping
			polygon_t polygon = create_polygon_from_triangle(
				face_vertices[0], face_vertices[1], face_vertices[2],
				mesh_face.a_uv, mesh_face.b_uv, mesh_face.c_uv,
				face_normals[0], face_normals[1], face_normals[2]
			);

			clip_polygon(&polygon);

			for (int i = 1; i < polygon.num_vertices - 1; ++i)
			{
				int index0 = 0;
				int index1 = i;
				int index2 = i + 1;

				triangle_t triangle = create_triangle(&polygon, index0, index1, index2);

				project_triangle(&triangle);

				if (num_triangles_to_render < MAX_TRIANGLES_PER_MESH)
				{
					triangles_to_render[num_triangles_to_render] = triangle;
					mesh->last_triangle_index = num_triangles_to_render;
					num_triangles_to_render += 1;
				}
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
	clear_z_buffer();
	clear_color_buffer(0xFF000000);

	draw_grid(0xFF333333);

	if (get_mesh_count() > 0)
	{
		int current_mesh_index = 0;
		mesh_t* current_mesh = get_mesh_item(current_mesh_index);

		for (int i = 0; i < num_triangles_to_render; i++)
		{
			if (current_mesh->last_triangle_index < i)
			{
				current_mesh_index++;
				current_mesh = get_mesh_item(current_mesh_index);
			}

			triangle_t triangle = triangles_to_render[i];

			if (rendering_mode & filled_mask)
			{
				fill_triangle(triangle, triangle.color);
			}

			if (rendering_mode & textured_mask)
			{
				draw_textured_triangle(triangle, &current_mesh->mesh_texture, rendering_mode & lighting_mask);

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
	}


	render_color_buffer();
}

void free_resources(void)
{
	destroy_window();
	free_mesh_container();
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

	free_resources();

	return 0;
}