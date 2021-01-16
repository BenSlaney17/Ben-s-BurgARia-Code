#include "VegDispenser.h"




VegDispenser::VegDispenser(CurrentVegetable veg_type, VegetablesContainer * veg_container, gef::Vector4 start_pos, const gef::Mesh * mesh, CollisionEngine * collisionEngine)
	:GameObject(start_pos, mesh, collisionEngine), veg_type_(veg_type), veg_container_(veg_container)
{
	is_interactable_ = true;
	interaction_text_ = veg_type_  == CurrentVegetable::Onion ? "Dispense Onion" : "Dispense Tomato";
}

VegDispenser::~VegDispenser()
{
	veg_container_ = NULL;
}

void VegDispenser::Interaction()
{
	// only dispense the veg if there is nothing on the chopping board
	if(veg_container_->GetCurrentVegetable() == CurrentVegetable::None)
		veg_container_->SetCurrentVegetable(veg_type_);
}
