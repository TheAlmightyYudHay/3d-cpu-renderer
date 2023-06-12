#pragma once

#include "Vector.h"
#include "triangle.h"
#include <vector>

extern "C" {
	#include "texture.h"
}

class Mesh
{
public:
	Mesh() 
		: mVertices(), mFaces(), mRotation(0, 0, 0), mScale(1.0f, 1.0f, 1.0f), 
		mTranslation(0.0f, 0.0f, 0.0f), mMeshTexture{ 0 }, mLastTriangleIndex{ 0 } 
	{}

	void AddVertex(const Vector3& vertex) { mVertices.push_back(vertex); }
	void AddFace(const face_t& face) { mFaces.push_back(face); }
	void SetTexture(const texture_t& texture) { mMeshTexture = texture; }
	texture_t& GetTexture() { return mMeshTexture; }
	void FreeTexture() { free_texture(&mMeshTexture); }
	void SetLastTriangleIndex(int index) { mLastTriangleIndex = index; }
	int GetLastTriangleIndex() const { return mLastTriangleIndex; }

	const Vector3& GetRotation() const { return mRotation; }
	const Vector3& GetScale() const { return mScale; }
	const Vector3& GetTranslation() const { return mTranslation; }

	void SetRotation(const Vector3& rotation) { mRotation = rotation; }
	void SetScale(const Vector3& scale) { mScale = scale; }
	void SetTranslation(const Vector3& translation) { mTranslation = translation; }

	const std::vector<Vector3>& GetVertices() const { return mVertices; }
	const std::vector<face_t>& GetFaces() const { return mFaces; }

private:
	std::vector<Vector3> mVertices;
	std::vector<face_t> mFaces;
	Vector3 mRotation;
	Vector3 mScale;
	Vector3 mTranslation;

	texture_t mMeshTexture;

	int mLastTriangleIndex;
};

class MeshContainer
{
public:
	MeshContainer() : mMeshes(), mMeshesCount(0) {}
	void LoadMeshData(const char* filename, const char* texture_path);
	void FreeMeshContainer();

	int GetMeshCount() const { return mMeshesCount; }

	//Does not check for out of bounds
	Mesh& GetMeshItem(int index) { return mMeshes[index]; }


private:
	std::vector<Mesh> mMeshes;
	int mMeshesCount;
};