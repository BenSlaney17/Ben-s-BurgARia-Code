#include "MyTransform.h"


MyTransform::MyTransform() :
	parent_(NULL),
	children_(NULL)
{

}

MyTransform::~MyTransform()
{
	parent_ = NULL;
	children_.clear();
}

void MyTransform::Init()
{
	local_transform_.SetIdentity();
	scale_ = gef::Vector4(1.f, 1.f, 1.f);
	rotationX_ = 0.f;
	rotationY_ = 0.f; 
	rotationZ_ = 0.f;
	position_ = gef::Vector4(0.f, 0.f, 0.f);

	parent_ = NULL;
	
	UpdateTransformationMatrix();
}

void MyTransform::Rotate(const float &xAngle, const float &yAngle, const float &zAngle)
{
	rotationX_ = gef::DegToRad(xAngle);
	rotationY_ = gef::DegToRad(yAngle);
	rotationZ_ = gef::DegToRad(zAngle);
}

void MyTransform::RotateBy(const float &xAngle, const float &yAngle, const float &zAngle)
{
	rotationX_ += gef::DegToRad(xAngle);
	rotationY_ += gef::DegToRad(yAngle);
	rotationZ_ += gef::DegToRad(zAngle);
}

void MyTransform::UpdateTransformationMatrix()
{
	// scale
	gef::Matrix44 scale;
	scale.Scale(scale_);

	// rotate x
	gef::Matrix44 rotX;
	rotX.RotationX(rotationX_);

	// rotate y
	gef::Matrix44 rotY;
	rotY.RotationY(rotationY_);

	// rotate z
	gef::Matrix44 rotZ;
	rotZ.RotationZ(rotationZ_);

	gef::Matrix44 trans = scale * rotX * rotY * rotZ;

	// translate
	trans.SetTranslation(position_);
	
	local_transform_ = trans;
	
	if (parent_)
		transform_world_ = trans * parent_->GetWorldMatrix();

	
}
