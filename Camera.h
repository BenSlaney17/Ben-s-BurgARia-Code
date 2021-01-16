#pragma once
#include "MyTransform.h"
#include <maths/vector4.h>

class Camera
{

public:

	Camera();
	~Camera();

	void Init();
	void Update(float frame_time_);

	const gef::Vector4& GetCameraEye() const { return camera_eye_; }
	const gef::Vector4& GetCameraLookat() const { return camera_lookat_; }
	const gef::Vector4& GetCameraUp() const { return camera_up_; }
	const float& GetCameraFov() const { return camera_fov_; }
	const float& GetNearPlane() const { return near_plane_; }
	const float& GetFarPlane() const { return far_plane_; }

	MyTransform transform_;

private:

	gef::Vector4 camera_eye_;
	gef::Vector4 camera_lookat_;
	gef::Vector4 camera_up_;
	float camera_fov_;
	float near_plane_;
	float far_plane_;
};

