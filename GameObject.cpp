#include "GameObject.h"
#include "graphics/renderer_3d.h"
#include "CollisionEngine.h"

GameObject::GameObject(gef::Vector4 start_pos, const gef::Mesh* mesh, CollisionEngine* collisionEngine)
{
	InitTransform(start_pos);

	velocity_ = gef::Vector4(0.f, 0.f, 0.f);

	mesh_.set_mesh(mesh);

	is_active_ = true;
	is_colliding_ = false;
	
	collisionEngine->SubscribeObject(this);
}

GameObject::~GameObject()
{
}

void GameObject::InitTransform(const gef::Vector4 pos = gef::Vector4(0.f, 0.f, 0.f))
{
	transform_.Init();
	transform_.SetPosition(pos);
	mesh_.set_transform(transform_.GetLocalMatrix());
}

bool GameObject::Update(float frame_rate)
{ 
	if (!is_active_) return false;

	// in physics update any position change based on velocity is applied
	PhysicsUpdate(frame_rate); 

	// Update transform and mesh transform
	TransformUpdate();
	return true;
}

void GameObject::Render(gef::Renderer3D* renderer)
{
	if (!is_active_) return;

	renderer->DrawMesh(mesh_);
}

void GameObject::PhysicsUpdate(float frame_time)
{
	// update the position based on velocity
	transform_.SetPosition(transform_.GetLocalPosition() + velocity_ * frame_time);
}

void GameObject::TransformUpdate()
{
	// update the Transformation Matrix
	transform_.UpdateTransformationMatrix();

	// update the transformation matrix of the mesh_
	mesh_.set_transform(transform_.GetWorldMatrix());
}


