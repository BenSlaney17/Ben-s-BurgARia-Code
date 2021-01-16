#pragma once
#include "GameObject.h"
class BurgerAssembly;
class VegetablesContainer;

class SlicedVegetable :
	public GameObject
{
public:
	SlicedVegetable(gef::Vector4 start_pos, const gef::Mesh* mesh, CollisionEngine* collision_engine, BurgerAssembly* burger_assembly, VegetablesContainer* veg_container);
	~SlicedVegetable();

	void Interaction() override;

	enum VegTypes
	{Onion, Tomato};

	void SetVegType(VegTypes value) { veg_type_ = value; }
private:
	BurgerAssembly * burger_assembly_;
	VegetablesContainer* veg_container_;
	VegTypes veg_type_;
};

