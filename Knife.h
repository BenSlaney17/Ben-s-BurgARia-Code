#pragma once
#include "GameObject.h"

namespace gef
{
	class AudioManager;
}

class Knife :
	public GameObject
{
public:
	Knife(gef::Vector4 start_pos, const gef::Mesh* mesh, CollisionEngine* collisionEngine);
	~Knife();

	bool Update(float frame_time);

	void OnCollisionStay(GameObject* otherObj);
	void OnCollisionExit(GameObject* otherObj);
	void ChopVegetable(GameObject * otherObj);
	

private:
	void UpdateVelocity(float f);
	bool IsChopping();


	gef::AudioManager* audio_manager_;

	Int32 background_noise_;

	gef::Vector4 cur_velocity_;
	gef::Vector4 last_frame_pos_;
	bool is_chopping_;

	const int frames_to_chop_ = 4;
	// counts the number of frames that the knife has been chopping the vegetable for
	int chopping_counter_;
};

