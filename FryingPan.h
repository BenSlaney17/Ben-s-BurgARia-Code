#pragma once
#include <deque>
#include "GameObject.h"
#include "audio\audio_manager.h"
class Burger;
class BurgerAssembly;



class FryingPan :
	public GameObject
{
public:
	FryingPan(gef::Vector4 start_pos, const gef::Mesh* mesh, CollisionEngine* collisionEngine, BurgerAssembly* burger_assembly, gef::AudioManager* audio_manager, gef::Platform& platform);
	~FryingPan();

	bool Update(float frame_time);


	void Interaction() override;

	void ResetBurgers();

	void AddBurger(Burger* burger) { burgers_.push_back(burger); }
private:
	bool HasBurgerIn() { return transform_.GetChildCount() > 0; }
	void AdjustSoundVolumeBasedOnPosition();
	bool IsBeingShaken();
	void UpdatePositionsDeque();
	void CheckNumOfBurgersAndUpdateTransforms();
	void UpdateBurgerTransforms(int positions_pos);
	bool ABurgerIsBurnt();
	bool ABurgerIsCooked();

	void SetFirstInactiveBurgerToActive();
	void SetLastBurgerInactive();

	std::deque<gef::Vector4> previous_positions_ = std::deque<gef::Vector4>(15);

	bool is_shaking_;
	float cooking_timer_;
	bool HasBurnt() { return cooking_timer_ >= time_to_burn_; }
	const float time_to_burn_ = 5.0f;

	BurgerAssembly* burger_assembly_;

	gef::AudioManager* audio_manager_;
	UInt32 frying_sound_;
	gef::VolumeInfo frying_volume_info_;

	std::vector<Burger*> burgers_;

	// burger positions
	std::vector<gef::Vector4> const burger_positions_
	{
		gef::Vector4(0.0f, 0.05f, 0.0f), gef::Vector4(0.0f, 0.0f, 0.0f), gef::Vector4(0.0f, 0.0f, 0.0f), gef::Vector4(0.0f, 0.0f, 0.0f),
		gef::Vector4(-0.1f, 0.05f, 0.0f), gef::Vector4(0.1f, 0.05f, 0.0f),gef::Vector4(0.0f, 0.0f, 0.0f), gef::Vector4(0.0f, 0.0f, 0.0f),
		gef::Vector4(-0.1f, 0.05f, 0.1f), gef::Vector4(0.1f, 0.05f, 0.1f), gef::Vector4(0.0f, 0.05f, -0.1f), gef::Vector4(0.0f, 0.0f, 0.0f),
		gef::Vector4(-0.1f, 0.05f, 0.1f), gef::Vector4(0.1f, 0.05f, 0.1f), gef::Vector4(-0.1f, 0.05f, -0.1f), gef::Vector4(0.1f, 0.05f, -0.1f)
	};
};

