#pragma once
#include <graphics/mesh_instance.h>
class GameObject;

static class CollisionFunctions
{
public:

	static bool IsCollidingAABB(const gef::MeshInstance& mesh1, const gef::MeshInstance& mesh2);
	static bool IsCollidingAABB(const GameObject& obj1, const GameObject& obj2);
	
	static bool IsCollidingSphere(const gef::MeshInstance& mesh1, const gef::MeshInstance& mesh2);
	static bool IsCollidingSphere(const GameObject& obj1, const GameObject& obj2);
};

