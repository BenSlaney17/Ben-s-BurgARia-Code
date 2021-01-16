#include "SlicedVegetable.h"
#include "BurgerAssembly.h"
#include "VegetablesContainer.h"

SlicedVegetable::SlicedVegetable(gef::Vector4 start_pos, const gef::Mesh * mesh, CollisionEngine * collision_engine, BurgerAssembly * burger_assembly, VegetablesContainer* veg_container)
	:GameObject(start_pos, mesh, collision_engine), burger_assembly_(burger_assembly), veg_container_(veg_container)
{
	is_interactable_ = true;
	interaction_text_ = "Send to container";
}

SlicedVegetable::~SlicedVegetable()
{
	burger_assembly_ = NULL;
	veg_container_ = NULL;
}

void SlicedVegetable::Interaction()
{

	switch (veg_type_)
	{
	case SlicedVegetable::Onion:
		burger_assembly_->AddOnion();
		// hacky way of setting the current veg to none once both sliced vegetables have been put away 
		if(burger_assembly_->GetNumOfOnions() % 2 == 0)
			veg_container_->SetCurrentVegetable(CurrentVegetable::None);

		break;
	case SlicedVegetable::Tomato:
		burger_assembly_->AddTomato();
		// hacky way of setting the current veg to none once both sliced vegetables have been put away 
		if (burger_assembly_->GetNumOfTomatoes() % 2 == 0)
			veg_container_->SetCurrentVegetable(CurrentVegetable::None);
		break;
	default:
		break;
	}

	SetActive(false);
}
