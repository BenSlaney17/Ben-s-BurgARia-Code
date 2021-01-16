#pragma once
#include <system/application.h>
#include <maths/vector4.h>
#include <maths/matrix44.h>
#include <graphics/mesh_instance.h>
#include "MyTransform.h"
#include "CollisionObject.h"

namespace gef
{
	class Mesh;
	class Renderer3D;
}

class CollisionEngine;

class GameObject : public CollisionObject 
{
public:

	enum ObjectType
	{O_Base, O_Pan, O_Knife, O_Vegetable};
	
	GameObject(gef::Vector4 start_pos, const gef::Mesh* mesh, CollisionEngine* collisionEngine);
	
	virtual ~GameObject();
	
	// if returns false then GameObject is not active
	virtual bool Update(float frame_rate);
	void Render( gef::Renderer3D* renderer);

	void SetVelocity(const gef::Vector4 velocity) { velocity_ = velocity; }
	gef::Vector4 Velocity() const { return velocity_; }

	void SetActive(bool isActive) { is_active_ = isActive; transform_.SetActive(isActive); }
	bool IsActive() { return is_active_; }
	
	virtual void Interaction(){}
	bool IsInteractable() { return is_interactable_; }
	const char* GetInteractionText() { return interaction_text_; }

	MyTransform transform_;
	gef::MeshInstance mesh_;

	ObjectType type_;
protected:
	void InitTransform(const gef::Vector4 pos);
	void PhysicsUpdate(float frame_time);
	void TransformUpdate();

	gef::Vector4 velocity_;

	bool is_active_;
	bool is_colliding_;

	bool is_interactable_ = false;
	const char* interaction_text_ = "";
};

