#include "Marker.h"
#include <sony_sample_framework.h>
#include <sony_tracking.h>


Marker::Marker(int _num)
{
	num_ = _num;
	// make it so the marker hasn't been seen at the start
	frames_since_last_seen_ = FRAMES_TILL_DISSAPEAR + 1;
	for (size_t i = 0; i < previous_transforms_.size(); i++)
	{
		previous_transforms_[i] = gef::Vector4::kZero;

	}
	Init();
}


Marker::~Marker()
{
}

void Marker::Update()
{
	UpdateTransformsVector();

	is_found_ = sampleIsMarkerFound(num_);
	// if is found then update transform
	if (is_found_)
		WhenFound();
	else
		WhenNotFound();
}

void Marker::UpdateTransformsVector()
{
	// Update the transforms vector
	previous_transforms_[2] = previous_transforms_[1];
	previous_transforms_[1] = previous_transforms_[0];
	previous_transforms_[0] = GetWorldMatrix().GetTranslation();
}
void Marker::WhenFound()
{
	frames_since_last_seen_ = 0;
	gef::Matrix44 marker_transform;
	sampleGetTransform(num_, &marker_transform);
	SetLocalTransform(marker_transform);
}

void Marker::WhenNotFound()
{
	frames_since_last_seen_ += 1;

	gef::Vector4 vec1 = previous_transforms_[1] - previous_transforms_[0];
	gef::Vector4 vec2 = previous_transforms_[2] - previous_transforms_[1];
	gef::Matrix44 newTransform = GetWorldMatrix();
	newTransform.SetTranslation(newTransform.GetTranslation() + (vec1 * 0.75f) + (vec2 * 0.25f));

	SetLocalTransform(newTransform);
	
	// predict next transform from previous 3 transforms
	/* OLD METHOD WITH TRANSFORMS
	// get the inverse transform for previous transforms 0 and 1
	gef::Matrix44 inverse_transform_0;
	inverse_transform_0.Inverse(previous_transforms_[0]);
	gef::Matrix44 inverse_transform_1 ;
	inverse_transform_1.Inverse(previous_transforms_[1]);

	// get the difference between transforms (0 and 1) and (1 and 2)
	gef::Matrix44 m0ToM1 = inverse_transform_0 * previous_transforms_[1];
	gef::Matrix44 m1ToM2 = inverse_transform_1 * previous_transforms_[2];

	//SetLocalTransform(GetWorldMatrix() * m0ToM1);
	*/
}
