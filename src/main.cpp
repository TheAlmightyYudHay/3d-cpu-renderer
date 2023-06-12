#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include "display.h"
#include "Vector.h"
#include "mesh.h"
#include "triangle.h"
#include "matrix.h"
#include "light.h"
#include "camera.h"
#include "clipping.h"
#include "configs.h"
#include "GlobalBuffers.h"

#define MAX_TRIANGLES_PER_MESH 10000
triangle_t triangles_to_render[MAX_TRIANGLES_PER_MESH] = {};
int num_triangles_to_render = 0;

bool is_running = false;
uint32_t previous_frame_time = 0;
float delta_time;

void setup(void)
{
	float aspecty = get_window_height() / (float)get_window_width();
	float aspectx = get_window_width() / (float)get_window_height();
	float fovy = M_PI * 60.0 / 180.0;
	float fovx = 2 * atan(aspectx * tan(fovy / 2.0));
	float zNear = 0.1;
	float zFar = 100.0;

	init_frustum_planes(fovy, fovx, zNear, zFar);

	Configs::GetInstance().Setup(fovy, aspecty, zNear, zFar);

	// Load hardcoded texture data
	//load_png_texture_data("./assets/drone.png");

	//load_cube_mesh_data();

	MeshContainer& meshContainer = GlobalBuffers::GetInstance().GetMeshContainer();

	meshContainer.LoadMeshData("./assets/cube.obj", "./assets/pikuma.png");
	meshContainer.LoadMeshData("./assets/drone.obj", "./assets/drone.png");
	meshContainer.LoadMeshData("./assets/cube.obj", "./assets/cube.png");
}

void process_input(void)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		Camera& camera = Configs::GetInstance().GetCamera();
		RenderingMode& renderingMode = Configs::GetInstance().GetRenderingMode();
		bool backfaceMode = renderingMode.IsSet(RenderingMode::Mode::BACKFACE_CULLING);

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
					renderingMode.Reset();
					renderingMode.SetMode(RenderingMode::Mode::WIREFRAME);
					renderingMode.SetMode(RenderingMode::Mode::VERTICES);
					if (backfaceMode)
						renderingMode.SetMode(RenderingMode::Mode::BACKFACE_CULLING);

					break;
				}
				if (event.key.keysym.sym == SDLK_2)
				{
					renderingMode.Reset();
					renderingMode.SetMode(RenderingMode::Mode::WIREFRAME);
					if (backfaceMode)
						renderingMode.SetMode(RenderingMode::Mode::BACKFACE_CULLING);

					break;
				}
				if (event.key.keysym.sym == SDLK_3)
				{
					renderingMode.Reset();
					renderingMode.SetMode(RenderingMode::Mode::FILLED);
					if (backfaceMode)
						renderingMode.SetMode(RenderingMode::Mode::BACKFACE_CULLING);

					break;
				}
				if (event.key.keysym.sym == SDLK_4)
				{
					renderingMode.Reset();
					renderingMode.SetMode(RenderingMode::Mode::FILLED);
					renderingMode.SetMode(RenderingMode::Mode::WIREFRAME);
					if (backfaceMode)
						renderingMode.SetMode(RenderingMode::Mode::BACKFACE_CULLING);

					break;
				}
				if (event.key.keysym.sym == SDLK_5)
				{
					renderingMode.Reset();
					renderingMode.SetMode(RenderingMode::Mode::TEXTURED);
					if (backfaceMode)
						renderingMode.SetMode(RenderingMode::Mode::BACKFACE_CULLING);

					break;
				}
				if (event.key.keysym.sym == SDLK_6)
				{
					renderingMode.Reset();
					renderingMode.SetMode(RenderingMode::Mode::TEXTURED);
					renderingMode.SetMode(RenderingMode::Mode::WIREFRAME);
					if (backfaceMode)
						renderingMode.SetMode(RenderingMode::Mode::BACKFACE_CULLING);

					break;
				}
				if (event.key.keysym.sym == SDLK_c)
				{
					renderingMode.ToggleMode(RenderingMode::Mode::BACKFACE_CULLING);
					break;
				}
				if (event.key.keysym.sym == SDLK_f)
				{
					renderingMode.ToggleMode(RenderingMode::Mode::LIGHTING);
					break;
				}
				if (event.key.keysym.sym == SDLK_UP)
				{
					camera.Move(Vector3{0, 5 * delta_time, 0});
					break;
				}
				if (event.key.keysym.sym == SDLK_DOWN)
				{
					camera.Move(Vector3{ 0, -5 * delta_time, 0 });
					break;
				}
				if (event.key.keysym.sym == SDLK_a)
				{
					camera.ChangeYaw(-3 * delta_time);
					break;
				}
				if (event.key.keysym.sym == SDLK_d)
				{
					camera.ChangeYaw(3 * delta_time);
					break;
				}
				if (event.key.keysym.sym == SDLK_q)
				{
					camera.ChangePitch(-3 * delta_time);
					break;
				}
				if (event.key.keysym.sym == SDLK_e)
				{
					camera.ChangePitch(3 * delta_time);
					break;
				}
				if (event.key.keysym.sym == SDLK_w)
				{
					Vector3 offset = camera.GetDirection() * (5.0 * delta_time);
					camera.Move(offset);
					break;
				}
				if (event.key.keysym.sym == SDLK_s)
				{
					Vector3 offset = camera.GetDirection() * (-5.0 * delta_time);
					camera.Move(offset);
					break;
				}
				break;
			default:
				break;
		}
	}
}

Vector3 calculate_face_normal(const Vector3& p1, const Vector3& p2, const Vector3 p3)
{
	return Vector3::Normalized(Vector3::Cross(p2 - p1, p3 - p1));
}

bool is_should_be_culled(const Vector3& pointOn, const Vector3& normal)
{
	if (!Configs::GetInstance().GetRenderingMode().IsSet(RenderingMode::Mode::BACKFACE_CULLING))
		return false;

	Vector3 origin = Vector3::Zero();//Since we work in view space, camera pos is always 0,0,0
	Vector3 camera_ray = origin - pointOn;
	return Vector3::Dot(normal, camera_ray) <= 0.0;
}

triangle_t create_triangle(polygon_t* polygon, int index0, int index1, int index2)
{

	triangle_t triangle{};

	triangle.points[0] = Vector4(polygon->vertices[index0]);
	triangle.points[1] = Vector4(polygon->vertices[index1]);
	triangle.points[2] = Vector4(polygon->vertices[index2]);

	triangle.texcoords[0] = polygon->texcoords[index0];
	triangle.texcoords[1] = polygon->texcoords[index1];
	triangle.texcoords[2] = polygon->texcoords[index2];

	triangle.normals[0] = polygon->normals[index0];
	triangle.normals[1] = polygon->normals[index1];
	triangle.normals[2] = polygon->normals[index2];

	triangle.color = 0xFFFFFFFF;

	if (Configs::GetInstance().GetRenderingMode().IsSet(RenderingMode::Mode::LIGHTING))
	{
		Vector3 face_normal = calculate_face_normal(polygon->vertices[index0], polygon->vertices[index1], polygon->vertices[index2]);
		float light_intencity_factor = Vector3::Dot(face_normal, Vector3::Negative(Configs::GetInstance().GetLight().GetViewDirection()));
		triangle.color = Light::ApplyIntensity(triangle.color, light_intencity_factor);
	}

	return triangle;
}

void project_triangle(triangle_t* triangle)
{
	for (int vIndex = 0; vIndex < 3; vIndex++)
	{
		Vector4 projected_point = Matrix4x4::MulVec4Project(Configs::GetInstance().GetProjectionMatrix(), triangle->points[vIndex]);

		// Remap x and y to viewport
		projected_point.SetX((projected_point.GetX() / 2.0 + 0.5) * (get_window_width() - 1));
		projected_point.SetY((projected_point.GetY() / 2.0 + 0.5) * (get_window_height() - 1));


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
	Camera& camera = Configs::GetInstance().GetCamera();

	Vector3 up_direction = { 0, 1, 0 };
	Vector3 target = { 0, 0, 10 };

	Matrix4x4 camera_yaw_rotation = Matrix4x4::MakeRotationY(camera.GetYaw());
	Matrix4x4 camera_pitch_rotation = Matrix4x4::MakeRotationX(camera.GetPitch());
	Matrix4x4 rotation_matrix = Matrix4x4::MulMat4(camera_yaw_rotation, camera_pitch_rotation);

	target = Vector3( Matrix4x4::MulVec4(rotation_matrix, Vector4(target)) );

	camera.SetDirection(target.Normalized());

	Matrix4x4 view_matrix = Matrix4x4::LookAt(camera.GetPosition(), target, up_direction);

	Vector3 lightDirection = Configs::GetInstance().GetLight().GetDirection();
	//////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////
	// Light in view space
	//////////////////////////////////////////////////////////////////////
	Vector3 new_view_direction = Vector3(Matrix4x4::MulVec4(view_matrix, Vector4 {
		lightDirection.GetX(),
		lightDirection.GetY(),
		lightDirection.GetZ(),
		0
	}));

	Configs::GetInstance().GetLight().SetView(new_view_direction.Normalized());
	//////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////
	// Handle meshes
	//////////////////////////////////////////////////////////////////////

	MeshContainer& meshContainer = GlobalBuffers::GetInstance().GetMeshContainer();

	for (int i = 0; i < meshContainer.GetMeshCount(); i++)
	{
		Mesh& mesh = meshContainer.GetMeshItem(i);

		mesh.SetLastTriangleIndex(0);

		const Vector3& currentRotation = mesh.GetRotation();
		const Vector3& currentTranslation = mesh.GetTranslation();
		const Vector3& currentScale = mesh.GetScale();

		mesh.SetRotation({
			currentRotation.GetX() + 0.25f * delta_time,
			currentRotation.GetY() + 0.25f * delta_time,
			currentRotation.GetZ() + 0.25f * delta_time
		});

		mesh.SetTranslation(Vector3(
			3 * i - 3,
			currentTranslation.GetY(),
			3 * i + 5
		));

		Matrix4x4 scale_matrix = Matrix4x4::MakeScale(currentScale.GetX(), currentScale.GetY(), currentScale.GetZ());
		Matrix4x4 translation_matrix = Matrix4x4::MakeTranslation(currentTranslation.GetX(), currentTranslation.GetY(), currentTranslation.GetZ());
		Matrix4x4 rotate_x_matrix = Matrix4x4::MakeRotationX(currentRotation.GetX());
		Matrix4x4 rotate_y_matrix = Matrix4x4::MakeRotationY(currentRotation.GetY());
		Matrix4x4 rotate_z_matrix = Matrix4x4::MakeRotationZ(currentRotation.GetZ());

		Matrix4x4 world_matrix = Matrix4x4::Identity();
		world_matrix = Matrix4x4::MulMat4(scale_matrix, world_matrix);
		world_matrix = Matrix4x4::MulMat4(rotate_z_matrix, world_matrix);
		world_matrix = Matrix4x4::MulMat4(rotate_y_matrix, world_matrix);
		world_matrix = Matrix4x4::MulMat4(rotate_x_matrix, world_matrix);
		world_matrix = Matrix4x4::MulMat4(translation_matrix, world_matrix);

		Matrix4x4 normal_matrix = Matrix4x4::Identity();
		normal_matrix = Matrix4x4::MulMat4(rotate_z_matrix, normal_matrix);
		normal_matrix = Matrix4x4::MulMat4(rotate_y_matrix, normal_matrix);
		normal_matrix = Matrix4x4::MulMat4(rotate_x_matrix, normal_matrix);

		auto vertices = mesh.GetVertices();
		auto faces = mesh.GetFaces();

		for (auto& face : faces)
		{
			Vector3 face_vertices[3] = {
				vertices[face.a],
				vertices[face.b],
				vertices[face.c],
			};

			Vector3 face_normals[3] = {
				face.a_normal,
				face.b_normal,
				face.c_normal
			};


			for (int vIndex = 0; vIndex < 3; vIndex++)
			{
				Vector4 transformed_vertex = Vector4(face_vertices[vIndex]);
				Vector4 transformed_normal = {
					face_normals[vIndex].GetX(),
					face_normals[vIndex].GetY(),
					face_normals[vIndex].GetZ(),
					0
				};

				transformed_vertex = Matrix4x4::MulVec4(world_matrix, transformed_vertex);
				transformed_vertex = Matrix4x4::MulVec4(view_matrix, transformed_vertex);

				transformed_normal = Matrix4x4::MulVec4(normal_matrix, transformed_normal);
				transformed_normal = Matrix4x4::MulVec4(view_matrix, transformed_normal);

				face_vertices[vIndex] = Vector3(transformed_vertex);
				face_normals[vIndex] = Vector3(transformed_normal);
			}

			face.normal = calculate_face_normal(face_vertices[0], face_vertices[1], face_vertices[2]);

			if (is_should_be_culled(face_vertices[0], face.normal))
			{
				continue;
			}


			// Clipping
			polygon_t polygon = create_polygon_from_triangle(
				face_vertices[0], face_vertices[1], face_vertices[2],
				face.a_uv, face.b_uv, face.c_uv,
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
					mesh.SetLastTriangleIndex(num_triangles_to_render);
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
		draw_rect(triangle.points[i].GetX() - 2, triangle.points[i].GetY() - 2, 4, 4, color);
	}
}

void render(void)
{
	clear_z_buffer();
	clear_color_buffer(0xFF000000);

	draw_grid(0xFF333333);

	const RenderingMode& renderingMode = Configs::GetInstance().GetRenderingMode();
	auto meshContainer = GlobalBuffers ::GetInstance().GetMeshContainer();


	if (meshContainer.GetMeshCount() > 0)
	{
		int current_mesh_index = 0;
		Mesh& currentMesh = meshContainer.GetMeshItem(current_mesh_index);

		for (int i = 0; i < num_triangles_to_render; i++)
		{
			if (currentMesh.GetLastTriangleIndex() < i)
			{
				current_mesh_index++;
				currentMesh = meshContainer.GetMeshItem(current_mesh_index);
			}

			triangle_t triangle = triangles_to_render[i];

			if (renderingMode.IsSet(RenderingMode::Mode::FILLED))
			{
				fill_triangle(triangle, triangle.color);
			}

			if (renderingMode.IsSet(RenderingMode::Mode::TEXTURED))
			{
				draw_textured_triangle(triangle, &currentMesh.GetTexture(), renderingMode.IsSet(RenderingMode::Mode::LIGHTING));

			}

			if (renderingMode.IsSet(RenderingMode::Mode::VERTICES))
			{
				draw_vertices(triangle, 0xFFFF0000);
			}
		}

		if (renderingMode.IsSet(RenderingMode::Mode::WIREFRAME))
		{
			for (int i = 0; i < num_triangles_to_render; i++)
			{
				triangle_t triangle = triangles_to_render[i];

				draw_wireframe(
					triangle.points[0].GetX(), triangle.points[0].GetY(),
					triangle.points[1].GetX(), triangle.points[1].GetY(),
					triangle.points[2].GetX(), triangle.points[2].GetY(),
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
	GlobalBuffers::GetInstance().GetMeshContainer().FreeMeshContainer();
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