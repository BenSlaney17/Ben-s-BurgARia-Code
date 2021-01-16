#include "CollisionEngine.h"
#include "Collision.h"
#include "GameObject.h"
#include "CollisionObject.h"
#include "graphics\mesh.h"
#include <system\platform.h>
#include "graphics\renderer_3d.h"
#include <sony_sample_framework.h>

CollisionEngine::CollisionEngine(gef::Platform * platform, gef::Renderer3D* renderer)
	: platform_(platform), renderer_(renderer)
{
	half_width_ = platform->width() * 0.5f;
	half_height_ = platform->height() * 0.5f;
}

CollisionEngine::~CollisionEngine()
{
	for (size_t i = 0; i < world_objects_.size(); i++)
	{
		world_objects_[i] = NULL;
	}
	world_objects_.clear();

	platform_ = NULL;
	renderer_ = NULL;
}

void CollisionEngine::Update()
{
	PreCollisionCheckUpdate();
	CollisionCheck();
	PostCollisionCheckUpdate();
	current_interactable_object_ = ScreenPointRaycast(gef::Vector2(half_width_, half_height_));
}

void CollisionEngine::PreCollisionCheckUpdate()
{
	// if an object is active, then call its PrePhysicsUpdate() function
	for (size_t i = 0; i < world_objects_.size(); i++)
	{
		if(world_objects_[i]->IsActive())
			world_objects_[i]->PrePhysicsUpdate();
	}
}

void CollisionEngine::CollisionCheck()
{
	// loop through all objects in world_objects_
	for (size_t i = 0; i < world_objects_.size() - 1; i++)
	{
		// if current object is not active then skip
		if (!world_objects_[i]->IsActive())
		{
			continue;
		}

		const gef::Mesh* firstMesh = world_objects_[i]->mesh_.mesh();
		for (size_t j = i + 1; j < world_objects_.size(); j++)
		{
			if (world_objects_[j]->IsActive() &&  
				CollisionFunctions::IsCollidingAABB(world_objects_[i]->mesh_, world_objects_[j]->mesh_))
			{
				world_objects_[i]->Colliding(world_objects_[j]);
				world_objects_[j]->Colliding(world_objects_[i]);
			}
		}
		
	}
}

void CollisionEngine::PostCollisionCheckUpdate()
{
	// if an object is active then call its PostPhysicsUpdate() function
	for (size_t i = 0; i < world_objects_.size(); i++)
	{
		if (world_objects_[i]->IsActive())
			world_objects_[i]->PostPhysicsUpdate();
	}
}

GameObject* CollisionEngine::ScreenPointRaycast(gef::Vector2 screen_point)
{
	gef::Vector4 start_pos;
	gef::Vector4 direction;

	GetScreenPositionRay(screen_point, start_pos, direction);

	// for each interactable gameobject check if the ray intersects
	for (size_t i = 0; i < world_objects_.size(); i++)
	{
		if (IsLookingAtInteractableObject(i, start_pos, direction))
		{
			return world_objects_[i];
		}
	}

	return NULL;
}

bool CollisionEngine::IsLookingAtInteractableObject(std::size_t &i, gef::Vector4 &start_pos, gef::Vector4 &direction)
{
	return world_objects_[i]->IsActive() && world_objects_[i]->IsInteractable() && IsHit(i, start_pos, direction);
}

bool CollisionEngine::IsHit(const std::size_t &i, gef::Vector4 &start_pos, gef::Vector4 &direction)
{
	gef::Vector4 sphereCentre(world_objects_[i]->transform_.GetWorldPosition());
	float sphereRadius = world_objects_[i]->transform_.GetScale().y() * 0.2f;
	gef::Vector4 m = start_pos - sphereCentre;
	float b = m.DotProduct(direction);
	float c = m.LengthSqr() - (sphereRadius * sphereRadius);
	
	// Exit if ray pointing away from sphere (b > 0)
	if (b > 0.0f)
	{
		return false;
	}

	// A negative discriminant corresponds to ray missing sphere
	float discr = b * b - c;

	return discr > 0.0f;
}

void CollisionEngine::GetScreenPositionRay(gef::Vector2 screen_point, gef::Vector4 & start_pos, gef::Vector4 & direction)
{
	gef::Vector2 ndc;

	ndc.x = (static_cast<float>(screen_point.x) - half_width_ )/ half_width_;
	ndc.y = (half_height_ - static_cast<float>(screen_point.y)) / half_height_;

	gef::Matrix44 projectionInverse;
	projectionInverse.Inverse(renderer_->view_matrix() * renderer_->projection_matrix());

	gef::Vector4 nearPoint, farPoint;
	nearPoint = gef::Vector4(ndc.x, ndc.y, -1.0f, 1.0f).TransformW(projectionInverse);
	farPoint = gef::Vector4(ndc.x, ndc.y, 1.0f, 1.0f).TransformW(projectionInverse);
	nearPoint /= nearPoint.w();
	farPoint /= farPoint.w();

	start_pos = gef::Vector4(nearPoint.x(), nearPoint.y(), nearPoint.z());
	direction = farPoint - nearPoint;
	direction.Normalise();
}


