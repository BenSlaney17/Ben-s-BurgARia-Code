#include "Collision.h"
#include <maths/aabb.h>
#include <maths/sphere.h>
#include <math.h>
#include "GameObject.h"
#include "graphics/mesh_instance.h"
#include <graphics/mesh.h>

bool CollisionFunctions::IsCollidingAABB(const gef::MeshInstance& meshInstance1, const gef::MeshInstance& meshInstance2)
{
	gef::Aabb aabb1 = meshInstance1.mesh()->aabb();
	gef::Aabb aabb2 = meshInstance2.mesh()->aabb();

	gef::Aabb aabb1_t = aabb1.Transform(meshInstance1.transform());
	gef::Aabb aabb2_t = aabb2.Transform(meshInstance2.transform());

	if (aabb1_t.max_vtx().x() > aabb2_t.min_vtx().x() &&
		aabb1_t.min_vtx().x() < aabb2_t.max_vtx().x() &&
		aabb1_t.max_vtx().y() > aabb2_t.min_vtx().y() &&
		aabb1_t.min_vtx().y() < aabb2_t.max_vtx().y() &&
		aabb1_t.max_vtx().z() > aabb2_t.min_vtx().z() &&
		aabb1_t.min_vtx().z() < aabb2_t.max_vtx().z())
	{
		return true;
	}

	return false;
}

bool CollisionFunctions::IsCollidingAABB(const GameObject& obj1, const GameObject& obj2)
{
	gef::Aabb aabb1 = obj1.mesh_.mesh()->aabb();
	gef::Aabb aabb2 = obj2.mesh_.mesh()->aabb();

	gef::Aabb aabb1_t = aabb1.Transform(obj1.mesh_.transform());
	gef::Aabb aabb2_t = aabb2.Transform(obj2.mesh_.transform());

	if (aabb1_t.max_vtx().x() > aabb2_t.min_vtx().x() &&
		aabb1_t.min_vtx().x() < aabb2_t.max_vtx().x() &&
		aabb1_t.max_vtx().y() > aabb2_t.min_vtx().y() &&
		aabb1_t.min_vtx().y() < aabb2_t.max_vtx().y() &&
		aabb1_t.max_vtx().z() > aabb2_t.min_vtx().z() &&
		aabb1_t.min_vtx().z() < aabb2_t.max_vtx().z())
	{
		return true;
	}

	return false;
}

bool CollisionFunctions::IsCollidingSphere(const gef::MeshInstance& meshInstance1, const gef::MeshInstance& meshInstance2)
{
	gef::Sphere sphere1 = meshInstance1.mesh()->bounding_sphere();
	gef::Sphere sphere2 = meshInstance2.mesh()->bounding_sphere();

	gef::Sphere sphere1_transformed = sphere1.Transform(meshInstance1.transform());
	gef::Sphere sphere2_transformed = sphere2.Transform(meshInstance2.transform());

	float combined_radii = sphere1_transformed.radius() + sphere2_transformed.radius();

	gef::Vector4 offset = sphere2_transformed.position() - sphere1_transformed.position();
	float distance = sqrt(pow(offset.x(), 2) + pow(offset.y(), 2) + pow(offset.z(), 2));

	return distance <= combined_radii;
}

bool CollisionFunctions::IsCollidingSphere(const GameObject& obj1, const GameObject& obj2)
{
	gef::Sphere sphere1 = obj1.mesh_.mesh()->bounding_sphere();
	gef::Sphere sphere2 = obj2.mesh_.mesh()->bounding_sphere();

	gef::Sphere sphere1_transformed = sphere1.Transform(obj1.mesh_.transform());
	gef::Sphere sphere2_transformed = sphere2.Transform(obj2.mesh_.transform());

	float combined_radii = sphere1_transformed.radius() + sphere2_transformed.radius();

	gef::Vector4 offset = sphere2_transformed.position() - sphere1_transformed.position();
	float distance = sqrt(pow(offset.x(), 2) + pow(offset.y(), 2) + pow(offset.z(), 2));

	return distance <= combined_radii;
}
