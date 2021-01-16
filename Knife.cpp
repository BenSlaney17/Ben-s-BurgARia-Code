#include "Knife.h"
#include "Vegetable.h"
#include "sony_sample_framework.h"
#include "audio\audio_manager.h"

Knife::Knife(gef::Vector4 start_pos, const gef::Mesh* mesh, CollisionEngine* collisionEngine)
	: GameObject(start_pos, mesh, collisionEngine)
{
	transform_.SetScale(0.4f);
	transform_.SetRotationZ(90.0f);
	transform_.SetPosition(gef::Vector4(0.0f, -0.075f, 0.05f));
	type_ = ObjectType::O_Knife;

	last_frame_pos_ = transform_.GetLocalPosition();
	chopping_counter_ = 0;

	//background_noise_ = audio_manager_->LoadMusic("", platform);
	//audio_manager_->PlayMusic();
}


Knife::~Knife()
{
	audio_manager_ = NULL;
}

bool Knife::Update(float frame_time)
{
	UpdateVelocity(frame_time);
	
	last_frame_pos_ = transform_.GetWorldPosition();

	GameObject::Update(frame_time);
	
	return true;
}

void Knife::UpdateVelocity(float frame_time)
{
	// update current velocity based on last frames position
	cur_velocity_ = (transform_.GetWorldPosition() - last_frame_pos_)/frame_time;
}


void Knife::OnCollisionStay(GameObject * otherObj)
{
	if (IsChopping())
	{
		//DEBUG_PRINTF("Knife Y velocity: %f", cur_velocity_.y());
		
		/*chopping_counter_++;
		if(chopping_counter_ == frames_to_chop_)*/
			ChopVegetable(otherObj);
	}
}

void Knife::OnCollisionExit(GameObject * otherObj)
{
	chopping_counter_ = 0;
}


void Knife::ChopVegetable(GameObject * otherObj)
{
	// make sure the type is vegetable
	if (otherObj->type_ != ObjectType::O_Vegetable) return;
	
	// chop the vegetable
	Vegetable* veg =  (Vegetable*)otherObj;
	veg->Chop();
}

const float chopSpeed = -0.6f;
bool Knife::IsChopping() { return cur_velocity_.y() < chopSpeed; }
