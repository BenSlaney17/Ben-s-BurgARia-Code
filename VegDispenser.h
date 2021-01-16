#pragma once
#include "GameObject.h"
#include "VegetablesContainer.h"

class VegetablesContainer;

class VegDispenser : public GameObject
{
public:
	VegDispenser(CurrentVegetable veg_type, VegetablesContainer* veg_container, gef::Vector4 start_pos, const gef::Mesh* mesh, CollisionEngine* collisionEngine);
	~VegDispenser();

	void Interaction() override;
private:
	CurrentVegetable veg_type_;
	VegetablesContainer* veg_container_;
};

