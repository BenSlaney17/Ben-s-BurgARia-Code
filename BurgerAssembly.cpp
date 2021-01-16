#include "BurgerAssembly.h"
#include "GameObject.h"
#include "graphics/mesh.h"
#include "graphics/scene.h"
#include "MeshLoading.h"
#include "OrderManager.h"

BurgerAssembly::BurgerAssembly(gef::Platform& platform, MyTransform* parent_transform, CollisionEngine* collision_engine, OrderManager* order_manager)
	: 
	burger_scene_(NULL),
	single_burger_mesh_(NULL),
	double_burger_mesh_(NULL),
	order_manager_(order_manager)
{
	num_of_burgers_ = 4;
	num_of_onions_ = 4;
	num_of_tomatoes_ = 4;

	MeshLoading::ReadSceneAndAssignFirstMesh(platform, "ChangedModels/burger_single.scn", &burger_scene_, &single_burger_mesh_);
	MeshLoading::ReadSceneAndAssignFirstMesh(platform, "ChangedModels/burger_double.scn", &burger_scene_, &double_burger_mesh_);

	for (size_t i = 0; i < max_num_of_burgers_; i++)
	{
		burgers_[i] = new GameObject(gef::Vector4(0, 0, 0), single_burger_mesh_, collision_engine);
		burgers_[i]->transform_.SetParent(parent_transform);
		burgers_[i]->transform_.SetScale(0.2f);
		burgers_[i]->transform_.SetRotationX(90.f);
		burgers_[i]->SetActive(false);
	}
}

BurgerAssembly::~BurgerAssembly()
{
	for (size_t i = 0; i < max_num_of_burgers_; i++)
	{
		delete burgers_[i];
		burgers_[i] = NULL;
	}
	
	delete burger_scene_;
	burger_scene_ = NULL;

	delete single_burger_mesh_;
	single_burger_mesh_ = NULL;

	delete double_burger_mesh_;
	double_burger_mesh_ = NULL;
}

void BurgerAssembly::Update(float frame_time)
{


	for (size_t i = 0; i < max_num_of_burgers_; i++)
	{
		burgers_[i]->SetActive(order_manager_->HasOrder() && order_manager_->GetFirstOrder().burgers_[i].is_complete_);
		burgers_[i]->Update(frame_time);
	}

	CheckForNumOfActiveBurgersAndUpdateTransforms();
}

void BurgerAssembly::CheckForNumOfActiveBurgersAndUpdateTransforms()
{
	int num_of_burgers = 0;
	for (int i = 0; i < max_num_of_burgers_; i++)
	{
		if (burgers_[i]->IsActive())
		{
			num_of_burgers++;
		}
	}
	if (num_of_burgers == 0) return;
	UpdateBurgerTransforms((num_of_burgers - 1) * 4);

}

void BurgerAssembly::UpdateBurgerTransforms(int num_of_burgers)
{
	for (int i = 0; i < max_num_of_burgers_; i++)
	{
		//Update child transform;
		burgers_[i]->transform_.SetPosition(burger_positions_[num_of_burgers + i]);
	}
}

void BurgerAssembly::Render(gef::Renderer3D* renderer)
{
	for (size_t i = 0; i < max_num_of_burgers_; i++)
	{
		burgers_[i]->Render(renderer);
	}
}

void BurgerAssembly::BuildBurger()
{
	if (order_manager_->HasOrder() == false)return;
	// get a pointer to the first order
	Order first_order = order_manager_->GetFirstOrder();

	// currently there are a max of 2 burgers per order. Get the vector position of the first that is not complete
	// if both burgers are complete then return
	int burgerNum = first_order.burgers_[0].is_complete_ ? (first_order.burgers_[1].is_complete_ ? -1 : 1) : 0;
	if (burgerNum == -1) return;
	
	if (!CanBuildBurger(first_order.burgers_[burgerNum].num_burgers_, first_order.burgers_[burgerNum].num_onions_, first_order.burgers_[burgerNum].num_tomatoes_)) return;

	// reduce num_of_item_ variables
	ReduceBurgerComponentVariables(first_order.burgers_[burgerNum].num_burgers_, first_order.burgers_[burgerNum].num_onions_, first_order.burgers_[burgerNum].num_tomatoes_);

	// generate burger 
	int nextAvailableBurger = GetFirstAvailableBurger();
	burgers_[nextAvailableBurger]->SetActive(true);
	// set the mesh to either single or double depending on numOfBurgers
	burgers_[nextAvailableBurger]->mesh_.set_mesh(first_order.burgers_[burgerNum].num_burgers_ == 1 ? single_burger_mesh_ : double_burger_mesh_);
	
	// update the burger mesh transform  
	burgers_[nextAvailableBurger]->Update(0);

	// tell the order that burger has been built
	first_order.BurgerComplete(burgerNum);

	order_manager_->SetFirstOrder(first_order);
}

void BurgerAssembly::SetAllBurgersInactive()
{
	for (size_t i = 0; i < max_num_of_burgers_; i++)
	{
		burgers_[i]->SetActive(false);
	}
}

int BurgerAssembly::GetFirstAvailableBurger()
{
	for (size_t i = 0; i < max_num_of_burgers_; i++)
	{
		if (!burgers_[i]->IsActive())
		{
			return i;
		}
	}
	return -1;
}

void BurgerAssembly::ReduceBurgerComponentVariables(int numOfBurgers, int numOfOnions, int numOfTomatoes)
{
	num_of_burgers_ -= numOfBurgers;
	num_of_onions_ -= numOfOnions;
	num_of_tomatoes_ -= numOfTomatoes;
}
