#pragma once
#include "GameObject.h"

class BurgerAssembly;
class BurgerAssemblyButton :
	public GameObject
{
public:
	BurgerAssemblyButton(gef::Mesh* mesh, CollisionEngine* collision_engine, BurgerAssembly* burger_assembly);
	~BurgerAssemblyButton();

	void Interaction() override;

private:
	BurgerAssembly * burger_assembly_;
};

