#include "Vegetable.h"
#include "VegetableParent.h"
Vegetable::Vegetable(gef::Vector4 start_pos, const gef::Mesh* mesh, CollisionEngine* collisionEngine)
	: GameObject(start_pos, mesh, collisionEngine)
{
	type_ = ObjectType::O_Vegetable;
}


Vegetable::~Vegetable()
{
	parent = NULL;
}

bool Vegetable::Update(float frame_time)
{
	GameObject::Update(frame_time);
	return true;
}

void Vegetable::Chop()
{
	parent->Chop();
}
