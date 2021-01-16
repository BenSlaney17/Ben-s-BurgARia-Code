#include "VegetableParent.h"
#include "Vegetable.h"

VegetableParent::VegetableParent(gef::Vector4 startPos, gef::Vector4 splitOffset, float scale, gef::Vector4 rotation, gef::Vector4 splitRot, 
	gef::Mesh* wholeMesh, gef::Mesh* halfMesh, CollisionEngine* collisionEngine, MyTransform* parent, BurgerAssembly* burger_assembly, VegetablesContainer* veg_container)
	:
	vegetable_(NULL),
	vegHalf1_(NULL),
	vegHalf2_(NULL)
{
	vegetable_ = new Vegetable(startPos, wholeMesh, collisionEngine);
	vegetable_->transform_.SetParent(parent);
	vegetable_->transform_.SetScale(scale);
	vegetable_->transform_.SetRotationX(rotation.x());
	vegetable_->SetActive(true);
	vegetable_->SetParent(this);

	vegHalf1_ = new SlicedVegetable(startPos - splitOffset, halfMesh, collisionEngine, burger_assembly, veg_container);
	vegHalf2_ = new SlicedVegetable(startPos + splitOffset, halfMesh, collisionEngine, burger_assembly, veg_container);

	vegHalf1_->transform_.SetParent(parent);
	vegHalf1_->SetActive(false);
	vegHalf1_->transform_.Rotate(splitRot.x(), splitRot.y(), splitRot.z());
	vegHalf1_->transform_.SetScale(scale);

	vegHalf2_->transform_.SetParent(parent);
	vegHalf2_->SetActive(false);
	vegHalf2_->transform_.Rotate(splitRot.x(), splitRot.y(), -splitRot.z());
	vegHalf2_->transform_.SetScale(scale);

	been_chopped_ = false;
}


VegetableParent::~VegetableParent()
{
	delete vegetable_;
	vegetable_ = NULL;

	delete vegHalf1_;
	vegHalf1_ = NULL;

	delete vegHalf2_;
	vegHalf2_ = NULL;
}

void VegetableParent::Update(float frame_time)
{
	if (been_chopped_)
	{
		vegHalf1_->Update(frame_time);
		vegHalf2_->Update(frame_time);
	}
	else
	{
		vegetable_->Update(frame_time);
	}
}

void VegetableParent::Render(gef::Renderer3D * renderer)
{
	if (been_chopped_)
	{
		vegHalf1_->Render(renderer);
		vegHalf2_->Render(renderer);
	}			   
	else		   
	{			   
		vegetable_->Render(renderer);
	}
}

void VegetableParent::SetActive(bool active)
{
	// if veg is being set to active then it will be the whole version being rendered
	if (active)
	{
		vegetable_->SetActive(active);
		been_chopped_ = false;
	}
	// if veg is being set to not active then it will be after it has been chopped
	else
	{
		vegetable_->SetActive(active);
		vegHalf1_->SetActive(active);
		vegHalf2_->SetActive(active);
	}
}

void VegetableParent::Chop()
{
	been_chopped_ = true;
	vegetable_->SetActive(false);
	vegHalf1_->SetActive(true);
	vegHalf1_->Update(0);
	vegHalf2_->SetActive(true);
	vegHalf2_->Update(0);
}
