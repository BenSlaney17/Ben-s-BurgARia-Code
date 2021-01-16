#pragma once
#include <vector>
#include <memory>
#include <maths\vector2.h>
#include <maths\vector4.h>

class GameObject;
class CollisionObject;
namespace gef
{
	class Platform;
	class Renderer3D;
}


class CollisionEngine
{
public:
	CollisionEngine(gef::Platform* platform, gef::Renderer3D* renderer);
	~CollisionEngine();
	void Update();

	void PreCollisionCheckUpdate();
	void CollisionCheck();
	void PostCollisionCheckUpdate();
	
	GameObject* GetCurrentInteractableObject() { return current_interactable_object_; }

	void SubscribeObject(GameObject* obj) { world_objects_.push_back(obj); }

private :
	GameObject * ScreenPointRaycast(gef::Vector2 screen_point);
	bool IsLookingAtInteractableObject(std::size_t &i, gef::Vector4 &start_pos, gef::Vector4 &direction);
	bool IsHit(const std::size_t &i, gef::Vector4 &start_pos, gef::Vector4 &direction);
	void GetScreenPositionRay(gef::Vector2 screen_point, gef::Vector4& start_pos, gef::Vector4& direction);

	std::vector<GameObject*> world_objects_;

	GameObject* current_interactable_object_;

	gef::Platform* platform_;
	float half_width_;
	float half_height_;

	gef::Renderer3D* renderer_;

};

