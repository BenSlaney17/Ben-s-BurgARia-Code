#pragma once
#include "maths\vector4.h"
#include "SlicedVegetable.h"

namespace gef
{
	class Renderer3D;
	class Mesh;
}

class CollisionEngine;

class MyTransform;
class Vegetable;
class BurgerAssembly;
class VegetablesContainer;

class VegetableParent
{
public:
	VegetableParent(gef::Vector4 startPos, gef::Vector4 splitOffset, float scale, gef::Vector4 vegRot, gef::Vector4 splitRot, 
		gef::Mesh* wholeMesh, gef::Mesh* halfMesh, CollisionEngine* collisionEngine, MyTransform* parent, BurgerAssembly* burger_assembly, VegetablesContainer* veg_container);
	~VegetableParent();

	void Update(float frame_time);
	void Render(gef::Renderer3D* renderer);
	void SetActive(bool active);
	void Chop();
	void SetVegType(SlicedVegetable::VegTypes veg_type) { vegHalf1_->SetVegType(veg_type); vegHalf2_->SetVegType(veg_type);}
private:
	bool been_chopped_;
	Vegetable * vegetable_;
	SlicedVegetable* vegHalf1_;
	SlicedVegetable* vegHalf2_;
};

