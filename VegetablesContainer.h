#pragma once
#include "maths\vector4.h"
namespace gef
{
	class Platform;
	class Renderer3D;
	class Mesh;
	class Scene;
}

class BurgerAssembly;
class VegetableParent;
class CollisionEngine;
class MyTransform;

enum CurrentVegetable
{None, Onion, Tomato};

class VegetablesContainer
{
public:
	VegetablesContainer(gef::Vector4 spawn_pos, gef::Platform& platform, CollisionEngine* collision_engine, MyTransform* parent_transform, BurgerAssembly* burger_assembly);
	~VegetablesContainer();

	void Update(float frame_time);
	void Render(gef::Renderer3D* renderer);
	void SetCurrentVegetable(CurrentVegetable cv);
	CurrentVegetable GetCurrentVegetable() { return cur_vegetable_; }
private:

	void Init(gef::Vector4 spawn_pos, gef::Platform& platform, CollisionEngine* collision_engine, MyTransform* parent_transform, BurgerAssembly* burger_assembly);

	CurrentVegetable cur_vegetable_;

	VegetableParent * onion_;
	gef::Mesh* onion_mesh_;
	gef::Mesh* half_onion_mesh_;
	gef::Scene* onion_scene_;
	gef::Scene* half_onion_scene_;

	VegetableParent* tomato_;
	gef::Mesh* tomato_mesh_;
	gef::Mesh* half_tomato_mesh_;
	gef::Scene* tomato_scene_;
	gef::Scene* half_tomato_scene_;
};

