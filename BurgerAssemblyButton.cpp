#include "BurgerAssemblyButton.h"
#include "BurgerAssembly.h"

BurgerAssemblyButton::BurgerAssemblyButton(gef::Mesh * mesh, CollisionEngine * collision_engine, BurgerAssembly* burger_assembly)
	:GameObject(gef::Vector4::kZero, mesh, collision_engine), burger_assembly_(burger_assembly)
{
	is_interactable_ = true;
	interaction_text_ = "Assemble Burger";

	transform_.SetScale(0.3f);
}

BurgerAssemblyButton::~BurgerAssemblyButton()
{
	burger_assembly_ = NULL;
}

void BurgerAssemblyButton::Interaction()
{
	burger_assembly_->BuildBurger();
}
