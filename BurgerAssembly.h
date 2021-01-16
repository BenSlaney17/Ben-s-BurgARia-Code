#pragma once
class MyTransform;
class GameObject;
class CollisionEngine;
#include <vector>
#include "maths\vector4.h"
namespace gef
{
	class Platform;
	class Mesh;
	class Scene;
	class Renderer3D;
}

class OrderManager;

class BurgerAssembly
{
public:
	BurgerAssembly(gef::Platform& platform, MyTransform* burger_parent, CollisionEngine* collision_engine, OrderManager* order_manager);
	~BurgerAssembly();

	void Update(float frame_time);

	void Render(gef::Renderer3D* renderer);

	void BuildBurger();
	void SetAllBurgersInactive();

	// checks that there are enough of each burger component to build the burger
	bool CanBuildBurger(int numOfBurgers = 1, int numOfOnions = 1, int numOfTomatoes = 1)
	{
		return numOfBurgers <= num_of_burgers_ && numOfOnions <= num_of_onions_ && numOfTomatoes <= num_of_tomatoes_;
	}

	void AddBurger() { num_of_burgers_++; }
	void AddBurgers(int num) { num_of_burgers_ += num; }
	void AddOnion() { num_of_onions_++; }
	void AddTomato() { num_of_tomatoes_++; }
	
	const int GetNumOfBurgers() { return num_of_burgers_; }
	const int GetNumOfOnions() { return num_of_onions_; }
	const int GetNumOfTomatoes() { return num_of_tomatoes_; }
private:
	void CheckForNumOfActiveBurgersAndUpdateTransforms();
	void UpdateBurgerTransforms(int num_of_burgers);
	void ReduceBurgerComponentVariables(int numOfBurgers, int numOfOnions, int numOfTomatoes);
	int GetFirstAvailableBurger();
	
	// can only ever have a max of 4 burgers assembled at once
	static const int max_num_of_burgers_ = 2;
	GameObject* burgers_[max_num_of_burgers_];
	gef::Mesh* single_burger_mesh_;
	gef::Mesh* double_burger_mesh_;
	gef::Scene* burger_scene_;
	
	OrderManager* order_manager_;

	int num_of_burgers_;
	int num_of_onions_;
	int num_of_tomatoes_;

	// burger positions
	 std::vector<gef::Vector4> const burger_positions_
	{
		gef::Vector4(0.0f, 0.1f, 0.05f), gef::Vector4(0.0f, 0.0f, 0.0f), gef::Vector4(0.0f, 0.0f, 0.0f), gef::Vector4(0.0f, 0.0f, 0.0f),
		gef::Vector4(-0.05f, 0.1f, 0.05f), gef::Vector4(0.05f, 0.1f, 0.05f),gef::Vector4(0.0f, 0.0f, 0.0f), gef::Vector4(0.0f, 0.0f, 0.0f),
		gef::Vector4(-0.1f, 0.05f, 0.1f), gef::Vector4(0.1f, 0.05f, 0.1f), gef::Vector4(0.0f, 0.05f, -0.1f), gef::Vector4(0.0f, 0.0f, 0.0f),
		gef::Vector4(-0.05f, 0.05f, 0.f), gef::Vector4(0.05f, 0.05f, 0.f), gef::Vector4(-0.05f, 0.15f, -0.f), gef::Vector4(0.05f, 0.15f, -0.f)
	};
};

