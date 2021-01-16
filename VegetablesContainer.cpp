#include "VegetablesContainer.h"
#include "VegetableParent.h"
#include "primitive_builder.h"
#include "MeshLoading.h"
#include "graphics\renderer_3d.h"
#include <graphics/mesh.h>
#include <system/platform.h>
#include "CollisionEngine.h"

VegetablesContainer::VegetablesContainer(gef::Vector4 spawn_pos, gef::Platform& platform, CollisionEngine* collision_engine, MyTransform* parent_transform, BurgerAssembly* burger_assembly)
	:onion_(NULL),
	onion_mesh_(NULL),
	half_onion_mesh_(NULL),
	onion_scene_(NULL),
	half_onion_scene_(NULL),
	tomato_(NULL),
	tomato_mesh_(NULL),
	half_tomato_mesh_(NULL),
	tomato_scene_(NULL),
	half_tomato_scene_(NULL)
{
	Init(spawn_pos, platform, collision_engine, parent_transform, burger_assembly);
}

VegetablesContainer::~VegetablesContainer()
{
	delete onion_;
	onion_ = NULL;

	delete onion_mesh_;
	onion_mesh_ = NULL;

	delete half_onion_mesh_;
	half_onion_mesh_ = NULL;

	delete onion_scene_;
	onion_scene_ = NULL;

	delete half_onion_scene_;
	half_onion_scene_ = NULL;

	delete tomato_;
	tomato_ = NULL;

	delete tomato_mesh_;
	tomato_mesh_ = NULL;

	delete half_tomato_mesh_;
	half_tomato_mesh_ = NULL;

	delete tomato_scene_;
	tomato_scene_ = NULL;

	delete half_tomato_scene_;
	half_tomato_scene_ = NULL;
}

void VegetablesContainer::Init(gef::Vector4 spawn_pos, gef::Platform& platform, CollisionEngine* collision_engine, MyTransform* parent_transform, BurgerAssembly* burger_assembly)
{
	// create onion 
	MeshLoading::ReadSceneAndAssignFirstMesh(platform, "ChangedModels/onion_whole.scn", &onion_scene_, &onion_mesh_);
	MeshLoading::ReadSceneAndAssignFirstMesh(platform, "ChangedModels/onion_half.scn", &half_onion_scene_, &half_onion_mesh_);
	gef::Aabb onionBoundingBox(gef::Vector4(-0.11f, 0.f, -0.12f), gef::Vector4(0.11f, 0.25f, 0.12f));
	gef::Aabb halfOnionBoundingBox(gef::Vector4(0.f, 0.f, -0.12f), gef::Vector4(0.11f, 0.25f, 0.12f));
	onion_mesh_->set_aabb(onionBoundingBox);
	half_onion_mesh_->set_aabb(halfOnionBoundingBox);
	onion_ = new VegetableParent(spawn_pos, gef::Vector4(0.0f, 0.0f, 0.1f), 1.f, gef::Vector4(0.f, 0.f, 0.f), gef::Vector4(0.0f, 0.0f, 90.0f),
		onion_mesh_, half_onion_mesh_, collision_engine, parent_transform, burger_assembly, this);
	onion_->SetVegType(SlicedVegetable::VegTypes::Onion);

	// create tomato 
	MeshLoading::ReadSceneAndAssignFirstMesh(platform, "ChangedModels/tomato_whole.scn", &tomato_scene_, &tomato_mesh_);
	MeshLoading::ReadSceneAndAssignFirstMesh(platform, "ChangedModels/tomato_slice.scn", &half_tomato_scene_, &half_tomato_mesh_);
	gef::Aabb tomatoBoundingBox(gef::Vector4(-0.11f, 0.f, -0.12f), gef::Vector4(0.11f, 0.25f, 0.12f));
	gef::Aabb halftomatoBoundingBox(gef::Vector4(0.f, 0.f, -0.12f), gef::Vector4(0.11f, 0.25f, 0.12f));
	tomato_mesh_->set_aabb(tomatoBoundingBox);
	half_tomato_mesh_->set_aabb(halftomatoBoundingBox);
	tomato_ = new VegetableParent(spawn_pos, gef::Vector4(0.0f, 0, 0.1F), 1.f, gef::Vector4(0.f, 0.f, 0.f), gef::Vector4(0.0f, 0.0f, 0.0f),
		tomato_mesh_, half_tomato_mesh_, collision_engine, parent_transform, burger_assembly, this);
	tomato_->SetVegType(SlicedVegetable::VegTypes::Tomato);
}

void VegetablesContainer::Update(float frame_time)
{
	switch (cur_vegetable_)
	{
	case None:
		break;
	case Onion:
		onion_->Update(frame_time);
		break;
	case Tomato:
		tomato_->Update(frame_time);
		break;
	default:
		break;
	}
}

void VegetablesContainer::Render(gef::Renderer3D * renderer)
{
	switch (cur_vegetable_)
	{
	case None:
		break;
	case Onion:
		onion_->Render(renderer);
		break;
	case Tomato:
		tomato_->Render(renderer);
		break;
	default:
		break;
	}
}

void VegetablesContainer::SetCurrentVegetable(CurrentVegetable cv)
{
	cur_vegetable_ = cv;

	switch (cur_vegetable_)
	{
	case None:
		onion_->SetActive(false);
		tomato_->SetActive(false);
		break;
	case Onion:
		onion_->SetActive(true);
		onion_->Update(0); 
		tomato_->SetActive(false);
		break;
	case Tomato:
		onion_->SetActive(false);
		tomato_->Update(0);
		tomato_->SetActive(true);
		break;
	default:
		break;
	}
}




