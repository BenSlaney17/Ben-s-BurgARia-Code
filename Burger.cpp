#include "Burger.h"

Burger::Burger(gef::Vector4 start_pos, gef::Mesh * uncooked_mesh, gef::Mesh * cooked_mesh, gef::Mesh * burnt_mesh, CollisionEngine * collisionEngine)
	:GameObject(start_pos, uncooked_mesh, collisionEngine), uncooked_mesh_(uncooked_mesh), cooked_mesh_(cooked_mesh), burnt_mesh_(burnt_mesh),
	cooked_amount_(0.0f), is_burnt_(false), is_cooked_(false)
{
}

Burger::~Burger()
{
	uncooked_mesh_ = NULL;
	cooked_mesh_ = NULL;
	burnt_mesh_ = NULL;
}

bool Burger::Update(float frame_time)
{
	GameObject::Update(frame_time);

	if (is_burnt_) return;
	
	cooked_amount_ += frame_time;
	// if enough time has passed, then burger has cooked
	if (cooked_amount_ >= time_to_cook_)
	{
		mesh_.set_mesh(cooked_mesh_);
		is_cooked_ = true;
	}
	// however, if too much time has passed then the burger is burnt
	if (cooked_amount_ >= time_to_cook_ * 2)
	{
		Burn();
	}

	return true;
}

void Burger::Burn()
{
	is_burnt_ = true;
	is_cooked_ = false;
	mesh_.set_mesh(burnt_mesh_);
}

void Burger::Reset()
{
	cooked_amount_ = 0.0f;
	is_burnt_ = false;
	is_cooked_ = false;
	mesh_.set_mesh(uncooked_mesh_);

}
