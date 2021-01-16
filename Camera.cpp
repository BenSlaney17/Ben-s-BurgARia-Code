#include "Camera.h"
#include "maths/math_utils.h"

Camera::Camera()
{

}

Camera::~Camera()
{
}

void Camera::Init()
{
	// initialise the camera settings
	camera_eye_ = gef::Vector4(5.0f, 5.0f, 315.0f);
	camera_lookat_ = gef::Vector4(0.0f, 0.0f, 0.0f);
	camera_up_ = gef::Vector4(0.0f, 1.0f, 0.0f);
	camera_fov_ = gef::DegToRad(45.0f);
	near_plane_ = 0.01f;
	far_plane_ = 1000.f;

	// setup the transform
	transform_.Init();
	transform_.SetPosition(camera_eye_);
}

void Camera::Update(float frame_time_)
{
	camera_eye_ = transform_.GetWorldPosition();
}

