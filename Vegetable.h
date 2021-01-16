#pragma once
#include "GameObject.h"

class VegetableParent;

class Vegetable :
	public GameObject
{
public:
	Vegetable(gef::Vector4 start_pos, const gef::Mesh* mesh, CollisionEngine* collisionEngine);
	~Vegetable();
	bool Update(float frame_time);
	void Chop();

	void SetParent(VegetableParent* par) { parent = par; }
private:
	VegetableParent * parent;

};

