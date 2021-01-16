#pragma once
#include <vector>
#include "maths\vector4.h"
// forward declarations
namespace gef
{
	class Platform;
	class Renderer3D;
	class Mesh;
	class Scene;
	class AudioManager;
}

namespace Ben
{
	class PlayerInput;
}

class PrimitiveBuilder;
class GameObject;
class Camera;
class CollisionEngine;
class Marker;
class FryingPan;
class Knife;
class VegetablesContainer;
class BurgerAssembly;
class VegDispenser;
class BurgerAssemblyButton;
class OrderManager;
class Burger;
enum CurrentVegetable;

class World
{
public:
	World(gef::Platform& platform, gef::AudioManager* audio_manager, PrimitiveBuilder* primitive_builder, Ben::PlayerInput* player_input, gef::Renderer3D* renderer, OrderManager* order_manager);
	~World();

	void CleanupMarkers();

	bool Update(float frame_time);

	void BurgerAssemblyUpdate(float frame_time);

	void FoodAndChoppingBoardMarkerUpdate(float frame_time);

	void SetFirstInactiveBurgerToActive();
	void SetLastBurgerInactive();

	void Render(gef::Renderer3D* renderer);
	Camera* GetCamera() const { return camera_; }

	// this purely exists so that interaction manager can access the collision engine
	CollisionEngine* GetCollisionEngine() { return collision_engine_; }
	BurgerAssembly* GetBurgerAssembly() { return burger_assembly_; }

private:

	void Init(gef::Platform& platform, Ben::PlayerInput* player_input, gef::Renderer3D* renderer, OrderManager* order_manager);
	void InitialiseBurger(const std::size_t &i);
	void CreateVegDispenser(VegDispenser *& veg_dispenser, CurrentVegetable veg_type, VegetablesContainer* veg_container, gef::Vector4 position, gef::Mesh* mesh, CollisionEngine* collision_engine_);
	void FryingPanMarkerUpdate(float frame_time);
	void KnifeMarkerUpdate(float frame_time);

	PrimitiveBuilder* primitive_builder_;
	CollisionEngine* collision_engine_;
	Ben::PlayerInput* input_;

	Camera* camera_;

	//markers
	Marker* marker_three_;
	Marker* marker_four_;
	Marker* marker_five_;
	Marker* marker_six_;

	// actual objects
	FryingPan* frying_pan_;
	gef::Scene* frying_pan_scene_;
	gef::Mesh* frying_pan_mesh_;

	Knife* knife_;
	gef::Scene* knife_scene_;
	gef::Mesh* knife_mesh_;

	std::vector<Burger*> burgers_;
	gef::Scene* burger_scene_;
	gef::Mesh* raw_burger_mesh_;
	gef::Mesh* cooked_burger_mesh_;
	gef::Mesh* burnt_burger_mesh_;

	VegetablesContainer* veg_container_;

	GameObject* chopping_board_;
	gef::Scene* board_scene_;
	gef::Mesh* board_mesh_;
	gef::Scene* model_scene1_;
	gef::Mesh* ball1_mesh_;

	VegDispenser* onion_dispenser_;
	VegDispenser* tomato_dispenser_;
	BurgerAssembly* burger_assembly_;

	BurgerAssemblyButton* burger_assembly_button;
	gef::Mesh* tray_mesh_;
	GameObject* tray_;
	gef::AudioManager* audio_manager_;
};

