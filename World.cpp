#include "World.h"
#include <system/platform.h>
#include "GameObject.h"
#include "graphics\primitive.h"
#include <graphics/scene.h>
#include "PlayerInput.h"
#include <maths/sphere.h>
#include <graphics/mesh.h>
#include "CollisionEngine.h"
#include <sony_sample_framework.h>
#include <sony_tracking.h>
#include "primitive_builder.h"
#include "Marker.h"
#include "BouncyThing.h"
#include "MeshLoading.h"
#include "FryingPan.h"	
#include "Knife.h"
#include "VegetablesContainer.h"
#include "BurgerAssembly.h"
#include "BurgerAssemblyButton.h"
#include "VegDispenser.h"
#include "Burger.h"

World::World(gef::Platform& platform, gef::AudioManager* audio_manager, PrimitiveBuilder* primitive_builder, Ben::PlayerInput* player_input, gef::Renderer3D* renderer, OrderManager* order_manager)
	:primitive_builder_(primitive_builder),
	audio_manager_(audio_manager),
	collision_engine_(NULL),
	camera_(NULL),
	input_(NULL),
	model_scene1_(NULL),
	marker_three_(NULL),
	marker_four_(NULL),
	marker_five_(NULL),
	marker_six_(NULL),
	frying_pan_(NULL),
	frying_pan_scene_(NULL),
	frying_pan_mesh_(NULL),
	knife_(NULL),
	knife_scene_(NULL),
	knife_mesh_(NULL),
	burgers_(NULL),
	burger_scene_(NULL),
	raw_burger_mesh_(NULL),
	cooked_burger_mesh_(NULL),
	burnt_burger_mesh_(NULL),
	veg_container_(NULL),
	chopping_board_(NULL),
	board_scene_(NULL),
	board_mesh_(NULL),
	burger_assembly_(NULL),
	onion_dispenser_(NULL),
	tomato_dispenser_(NULL),
	burger_assembly_button(NULL),
	tray_mesh_(NULL),
	tray_(NULL)
{
	Init(platform, player_input, renderer, order_manager);	
}

World::~World()
{
	primitive_builder_ = NULL;

	delete camera_;
	camera_ = NULL;

	delete collision_engine_;
	collision_engine_ = NULL;

	input_ = NULL;

	delete model_scene1_;
	model_scene1_ = NULL;

	CleanupMarkers();

	delete frying_pan_;
	frying_pan_ = NULL;
	
	delete frying_pan_scene_;
	frying_pan_scene_ = NULL;

	delete frying_pan_mesh_;
	frying_pan_mesh_ = NULL;

	delete knife_;
	knife_ = NULL;

	delete knife_scene_;
	knife_scene_ = NULL;

	delete knife_mesh_;
	knife_mesh_ = NULL;

	for (size_t i = 0; i < burgers_.size(); i++)
	{
		delete burgers_[i];
		burgers_[i] = NULL;
	}
	burgers_.clear();

	delete burger_scene_;
	burger_scene_ = NULL;

	delete raw_burger_mesh_;
	raw_burger_mesh_ = NULL;

	delete cooked_burger_mesh_;
	cooked_burger_mesh_ = NULL;

	delete burnt_burger_mesh_;
	burnt_burger_mesh_ = NULL;

	delete veg_container_;
	veg_container_ = NULL;

	delete chopping_board_;
	chopping_board_ = NULL;

	delete board_scene_;
	board_scene_ = NULL;

	delete board_mesh_;
	board_mesh_ = NULL;

	delete burger_assembly_;
	burger_assembly_ = NULL;

	delete onion_dispenser_;
	onion_dispenser_ = NULL;

	delete tomato_dispenser_;
	tomato_dispenser_ = NULL;

	delete burger_assembly_button;
	burger_assembly_button = NULL;
	
	delete tray_mesh_;
	tray_mesh_ = NULL;

	delete tray_;
	tray_ = NULL;
}

void World::CleanupMarkers()
{
	delete marker_three_;
	marker_three_ = NULL;

	delete marker_four_;
	marker_four_ = NULL;

	delete marker_five_;
	marker_five_ = NULL;

	delete marker_six_;
	marker_six_ = NULL;
}

void World::Init(gef::Platform& platform, Ben::PlayerInput* player_input, gef::Renderer3D* renderer, OrderManager* order_manager)
{
	marker_three_ = new Marker(2);
	marker_four_ = new Marker(3);
	marker_five_ = new Marker(4);
	marker_six_ = new Marker(5);

	// create collision engine
	collision_engine_ = new CollisionEngine(&platform, renderer);

	// assign player input
	input_ = player_input;
		
	burger_assembly_ = new BurgerAssembly(platform, marker_three_, collision_engine_, order_manager);

	// create frying pan 
	MeshLoading::ReadSceneAndAssignFirstMesh(platform, "ChangedModels/fryingPan.scn", &frying_pan_scene_, &frying_pan_mesh_);
	frying_pan_ = new FryingPan(gef::Vector4(0,-0.025f,0.01f), frying_pan_mesh_, collision_engine_, burger_assembly_, audio_manager_, platform);
	frying_pan_->transform_.SetParent(marker_four_);

	// create knife
	MeshLoading::ReadSceneAndAssignFirstMesh(platform, "ChangedModels/knife_2.scn", &knife_scene_, &knife_mesh_);
	gef::Aabb knifeBoundingBox(gef::Vector4(-0.3f, 0.0f, -0.1f), gef::Vector4(0.3f, 0.02f, 0.1f));
	knife_mesh_->set_aabb(knifeBoundingBox);
	knife_ = new Knife(gef::Vector4(0.0f, 0.1f, 0.0f), knife_mesh_, collision_engine_);
	knife_->transform_.SetParent(marker_six_);

	//create burgers
	MeshLoading::ReadSceneAndAssignFirstMesh(platform, "ChangedModels/burger_patty_raw.scn", &burger_scene_, &raw_burger_mesh_);
	MeshLoading::ReadSceneAndAssignFirstMesh(platform, "ChangedModels/burger_patty_cooked.scn", &burger_scene_, &cooked_burger_mesh_);
	MeshLoading::ReadSceneAndAssignFirstMesh(platform, "ChangedModels/burger_patty_burnt.scn", &burger_scene_, &burnt_burger_mesh_);
	burgers_.resize(4);

	for (size_t i = 0; i < burgers_.size(); i++)
	{
		InitialiseBurger(i);
	}
		
	burger_assembly_button = new BurgerAssemblyButton(cooked_burger_mesh_, collision_engine_, burger_assembly_);
	burger_assembly_button->transform_.SetParent(marker_three_);
	MeshLoading::ReadSceneAndAssignFirstMesh(platform, "ChangedModels/plate_rectangle.scn", &burger_scene_, &tray_mesh_);
	tray_ = new GameObject(gef::Vector4(0.f, 0.15f, 0.f), tray_mesh_, collision_engine_);
	tray_->transform_.Rotate(90,0,0);
	tray_->transform_.SetScale(0.3f);
	tray_->transform_.SetParent(marker_three_);

	// create board
	MeshLoading::ReadSceneAndAssignFirstMesh(platform, "ChangedModels/chopping_board.scn", &board_scene_, &board_mesh_);
	chopping_board_ = new GameObject(gef::Vector4(0.15f, -0.1f, -0.05f), board_mesh_, collision_engine_);
	chopping_board_->transform_.SetParent(marker_five_);
	chopping_board_->transform_.SetScale(0.5f);
	chopping_board_->transform_.Rotate(90.f, 0.f, 90.f);

	veg_container_ = new VegetablesContainer(gef::Vector4(0.f, 0.1f, 0.f), platform, collision_engine_, &chopping_board_->transform_, burger_assembly_);
	veg_container_->SetCurrentVegetable(CurrentVegetable::None);
	
	CreateVegDispenser(onion_dispenser_, CurrentVegetable::Onion, veg_container_, gef::Vector4(0.0f, 0.0f, 0.1f), cooked_burger_mesh_, collision_engine_);
	CreateVegDispenser(tomato_dispenser_, CurrentVegetable::Tomato, veg_container_, gef::Vector4(0.2f, 0.0f, 0.1f), cooked_burger_mesh_, collision_engine_);
}

void World::InitialiseBurger(const std::size_t &i)
{
	burgers_[i] = new Burger(gef::Vector4(0, 0.f, 0.0f), raw_burger_mesh_, cooked_burger_mesh_, burnt_burger_mesh_, collision_engine_);
	burgers_[i]->transform_.SetScale(0.5f);
	burgers_[i]->transform_.SetParent(&frying_pan_->transform_);
	burgers_[i]->SetActive(true);
	frying_pan_->AddBurger(burgers_[i]);
}

void World::CreateVegDispenser(VegDispenser *& veg_dispenser, CurrentVegetable veg_type, VegetablesContainer* veg_container, gef::Vector4 position, gef::Mesh* mesh, CollisionEngine* collision_engine_)
{
	veg_dispenser = new VegDispenser(veg_type, veg_container, position, mesh, collision_engine_);
	veg_dispenser->transform_.SetParent(marker_five_);
	veg_dispenser->transform_.SetScale(0.3f);
	veg_dispenser->transform_.Rotate(0.f, 0.f, 0.f);
}

bool World::Update(float frame_time)
{
	FryingPanMarkerUpdate(frame_time);
	FoodAndChoppingBoardMarkerUpdate(frame_time);
	KnifeMarkerUpdate(frame_time);
	BurgerAssemblyUpdate(frame_time);

	collision_engine_->Update();
}

void World::BurgerAssemblyUpdate(float frame_time)
{
	marker_three_->Update();
	if (marker_three_->IsFound())
	{
		burger_assembly_button->Update(frame_time);
		burger_assembly_->Update(frame_time);
		tray_->Update(frame_time);
	}
}

void World::FryingPanMarkerUpdate(float frame_time)
{
	marker_four_->Update();
	if (marker_four_->IsFound())
	{
		frying_pan_->SetActive(true);
		//frying_pan_->transform_.RotateBy(20.f * frame_time, 0.f, 0.f);
		frying_pan_->Update(frame_time);

		if (input_->IsInputPressed("Down"))
		{
			SetLastBurgerInactive();
		}
		if (input_->IsInputPressed("Up"))
		{
			SetFirstInactiveBurgerToActive();
		}

		for (size_t i = 0; i < burgers_.size(); i++)
		{
			//burgers_[i]->SetActive(true);
			burgers_[i]->Update(frame_time);
		}
			
	}
	else
	{
		frying_pan_->SetActive(false);
		for (size_t i = 0; i < burgers_.size(); i++)
		{
			//burgers_[i]->SetActive(false);
		}
	}
}

void World::FoodAndChoppingBoardMarkerUpdate(float frame_time)
{
	marker_five_->Update();
	if (marker_five_->IsFound())
	{
		veg_container_->Update(frame_time);
		chopping_board_->Update(frame_time);
		onion_dispenser_->Update(frame_time);
		tomato_dispenser_->Update(frame_time);
	}
}

void World::SetFirstInactiveBurgerToActive()
{
	for (size_t i = 0; i < burgers_.size(); i++)
	{
		if (!burgers_[i]->IsActive())
		{
			burgers_[i]->SetActive(true);
			return;
		}
	}
}

void World::SetLastBurgerInactive()
{
	for (size_t i = 0; i < burgers_.size(); i++)
	{
		if (!burgers_[i]->IsActive())
		{
			i--;
			burgers_[i]->SetActive(false);
			return;
		}
	}
	//last one is active
	int num = burgers_.size() - 1;
	burgers_[num]->SetActive(false);
}

void World::KnifeMarkerUpdate(float frame_time)
{
	marker_six_->Update();
	if (marker_six_->IsFound())
	{
		knife_->SetActive(true);
		knife_->Update(frame_time);
	}
	else
	{
		knife_->SetActive(false);
	}
}

void World::Render(gef::Renderer3D * renderer_3d)
{
	if (marker_three_->IsFound())
	{
		burger_assembly_button->Render(renderer_3d);
		burger_assembly_->Render(renderer_3d);
		tray_->Render(renderer_3d);
	}

	if (marker_four_->IsFound())
	{
		//cube_->Render(renderer_3d);
		frying_pan_->Render(renderer_3d);
		for (size_t i = 0; i < burgers_.size(); i++)
		{
			burgers_[i]->Render(renderer_3d);
		}
	}
		
	if (marker_five_->IsFound())
	{
		veg_container_->Render(renderer_3d);
		chopping_board_->Render(renderer_3d);
		onion_dispenser_->Render(renderer_3d);
		tomato_dispenser_->Render(renderer_3d);
	}

	if (marker_six_->IsFound())
	{
		knife_->Render(renderer_3d);
	}

}
