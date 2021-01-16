#pragma once
#include "GameObject.h"


class Burger :
	public GameObject
{
public:
	Burger(gef::Vector4 start_pos,  gef::Mesh* uncooked_mesh,  gef::Mesh* cooked_mesh,  gef::Mesh* burnt_mesh, CollisionEngine* collisionEngine);
	~Burger();

	bool Update(float frame_time);

	void Burn();

	void Reset();
	bool IsCooked() { return is_cooked_; }
	bool IsBurnt() { return is_burnt_; }
private:

	float cooked_amount_;
	// it takes 10 seconds to cook the burger
	const float time_to_cook_ = 10.0f;
	bool is_cooked_;
	bool is_burnt_;

	gef::Mesh* uncooked_mesh_;
	gef::Mesh* cooked_mesh_;
	gef::Mesh* burnt_mesh_;
};

