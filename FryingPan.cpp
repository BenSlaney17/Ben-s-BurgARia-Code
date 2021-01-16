#include "FryingPan.h"
#include "sony_sample_framework.h"
#include <audio\audio_manager.h>
#include "BurgerAssembly.h"
#include "Burger.h"
#include <maths\vector2.h>

FryingPan::FryingPan(gef::Vector4 start_pos, const gef::Mesh* mesh, CollisionEngine* collisionEngine, BurgerAssembly* burger_assembly, gef::AudioManager* audio_manager, gef::Platform& platform)
: GameObject(start_pos, mesh, collisionEngine), burger_assembly_(burger_assembly), burgers_(std::vector<Burger*>()),
	audio_manager_(audio_manager)
{
	transform_.SetScale(0.2f);
	transform_.SetRotationX(90.f);
	transform_.SetRotationZ(270.f);
	is_shaking_ = false;

	is_interactable_ = true;
	interaction_text_ = "Remove burgers";

	frying_sound_ = 0;
	frying_volume_info_.volume = 0.0f;
	audio_manager_->SetSampleVoiceVolumeInfo(frying_sound_, frying_volume_info_);
	audio_manager_->PlaySample(frying_sound_, true);
}


FryingPan::~FryingPan()
{
	burger_assembly_ = NULL;
	for (size_t i = 0; i < burgers_.size(); i++)
	{
		burgers_[i] = NULL;
	}
	burgers_.clear();
}

bool FryingPan::Update(float frame_time)
{
	GameObject::Update(frame_time);
	
	UpdatePositionsDeque();
	
	// check if moving	
	is_shaking_ = IsBeingShaken();

	cooking_timer_ += frame_time;

	// pan was shalen when it was needed
	if (is_shaking_)
	{
		cooking_timer_ = 0.0f;
	}

	AdjustSoundVolumeBasedOnPosition();
	
	if (HasBurnt())
	{
		for (size_t i = 0; i < burgers_.size(); i++)
		{
			burgers_[i]->Burn();
		}
		cooking_timer_ = 0.0f;
	}
	
	CheckNumOfBurgersAndUpdateTransforms();

	interaction_text_ = ABurgerIsBurnt() ? "Remove burnt burgers" : ABurgerIsCooked() ? "Remove cooked burgers" : "Do nothing yet";

}

void FryingPan::AdjustSoundVolumeBasedOnPosition()
{
	const float xPos = abs( transform_.GetWorldPosition().x());
	const float yPos = abs(transform_.GetWorldPosition().y());
	gef::Vector2 posVec = gef::Vector2(xPos, yPos);
	frying_volume_info_.volume = 1.0f - 3.5f * posVec.Length();
	audio_manager_->SetSampleVoiceVolumeInfo(frying_sound_, frying_volume_info_);
}

void FryingPan::Interaction()
{
	// check for burnt burger
	if (ABurgerIsBurnt())
	{
		ResetBurgers();
	}
	else if (ABurgerIsCooked())
	{
		// add burgers to burger assembly
		burger_assembly_->AddBurgers(4);
		ResetBurgers();
	}
}

void FryingPan::ResetBurgers()
{
	for (size_t i = 0; i < burgers_.size(); i++)
	{
		burgers_[i]->Reset();
	}
	cooking_timer_ = 0.0f;
}

bool FryingPan::ABurgerIsBurnt()
{
	for (size_t i = 0; i < burgers_.size(); i++)
	{
		if (burgers_[i]->IsBurnt())
		{
			return true;
		}
	}
	return false;
}

bool FryingPan::ABurgerIsCooked()
{
	for (size_t i = 0; i < burgers_.size(); i++)
	{
		if (burgers_[i]->IsCooked())
		{
			return true;
		}
	}
	return false;
}

void FryingPan::CheckNumOfBurgersAndUpdateTransforms()
{
	// mathematical way of determining from what position in the positions array to look 
	// this is nicer than using a switch 

	int num_of_burgers = 0;
	for (int i = 0; i < transform_.GetChildCount(); i++)
	{
		if (transform_.GetChild(i)->IsActive())
		{
			num_of_burgers++;
		}
	}
	if (num_of_burgers == 0) return;
	UpdateBurgerTransforms((num_of_burgers - 1) * 4);
}

void FryingPan::UpdateBurgerTransforms(int positions_pos)
{
	for (int i = 0; i < transform_.GetChildCount(); i++)
	{
		//Update child transform;
		transform_.GetChild(i)->SetPosition(burger_positions_[positions_pos + i]);
	}
}

bool FryingPan::IsBeingShaken()
{
	float zDist1 = previous_positions_[7].z() - previous_positions_[0].z();
	float zDist2 = previous_positions_[15].z() - previous_positions_[7].z();
	const float distToBeShaking = 0.025f;
	return fabs(zDist1) > distToBeShaking && fabs(zDist2) > distToBeShaking;
}

void FryingPan::UpdatePositionsDeque()
{
	const int max_positions = 15;
	 
	if (previous_positions_.size() == max_positions)
		previous_positions_.pop_back();

	previous_positions_.push_front(transform_.GetWorldPosition());
}

void FryingPan::SetFirstInactiveBurgerToActive()
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

void FryingPan::SetLastBurgerInactive()
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