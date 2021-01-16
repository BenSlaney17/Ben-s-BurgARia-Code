#pragma once
#include "MyTransform.h"

class Marker :
	public MyTransform
{
public:
	Marker(int num);
	~Marker();

	void Update();

	void UpdateTransformsVector();

	void WhenFound();

	void WhenNotFound();

	const int Num() { return num_; }
	
	#define FRAMES_TILL_DISSAPEAR 15
	const bool IsFound() { return frames_since_last_seen_ <= FRAMES_TILL_DISSAPEAR; }

private:
	int num_;
	int frames_since_last_seen_;
	bool is_found_;

	std::vector<gef::Vector4> previous_transforms_{ gef::Vector4() , gef::Vector4() , gef::Vector4() };
};

