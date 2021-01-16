#include "MeshLoading.h"
#include <graphics/scene.h>

void MeshLoading::ReadSceneAndAssignFirstMesh(gef::Platform & platform, const char * filename, gef::Scene ** scene, gef::Mesh ** mesh)
{
	gef::Scene* scn = new gef::Scene();
	scn->ReadSceneFromFile(platform, filename);

	// we do want to render the data stored in the scene file so lets create the materialse from the material data present in the scene file
	scn->CreateMaterials(platform);

	*mesh = GetFirstMesh(platform, scn);
	*scene = scn;
}

gef::Mesh * MeshLoading::GetFirstMesh(gef::Platform & platform, gef::Scene * scene)
{
	gef::Mesh* mesh = NULL;

	if (scene)
	{
		// now check to see if there is any mesh_ data in the file, f so lets create a mesh_ from it
		if (scene->mesh_data.size() > 0)
			mesh = scene->CreateMesh(platform, scene->mesh_data.front());
	}
	return mesh;
}
