#pragma once

namespace gef
{
	class Platform;
	class Renderer3D;
	class Mesh;
	class Scene;
}

static class MeshLoading
{
public:
	static void ReadSceneAndAssignFirstMesh(gef::Platform& platform, const char* filename, gef::Scene** scene, gef::Mesh** mesh);
	static gef::Mesh* GetFirstMesh(gef::Platform& platform, gef::Scene* scene);
};

