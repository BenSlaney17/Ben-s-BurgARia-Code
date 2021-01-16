#pragma once
#include <system/application.h>
#include <maths/vector4.h>
#include <maths/matrix44.h>
#include <vector>
#include "maths/math_utils.h"

class MyTransform
{

public:
	
	MyTransform();
	~MyTransform();
	void Init();

	void SetLocalTransform(const gef::Matrix44 transform) { local_transform_ = transform; }

	// get matrices in both local and world space
	const gef::Matrix44& GetLocalMatrix()		{ return local_transform_; }
	const gef::Matrix44 GetWorldMatrix()		{ return (parent_) ?  transform_world_ : local_transform_;}

	void SetPosition(const gef::Vector4 pos)	{ position_ = pos; }
	const gef::Vector4 GetWorldPosition()		
	{ 
		return (parent_) ? transform_world_.GetTranslation() : position_;
	}
	const gef::Vector4 GetLocalPosition()		{ return position_; }

	void SetRotationX(const float rot)			{ rotationX_ = gef::DegToRad(rot); }
	float GetRotationX() const					{ return rotationX_; }
	void SetRotationY(const float rot)			{ rotationY_ = gef::DegToRad(rot); }
	float GetRotationY() const					{ return rotationY_; }
	void SetRotationZ(const float rot)			{ rotationZ_ = gef::DegToRad(rot); }
	float GetRotationZ() const					{ return rotationZ_; }
	
	void SetScale(const gef::Vector4 scale)		{ scale_ = scale; }
	void SetScale(const float scalarValue)		{ scale_ = gef::Vector4(scalarValue, scalarValue, scalarValue); }
	gef::Vector4 GetScale() const				{ return scale_; }

	// move by Vector4
	void Translate(const gef::Vector4 translation) { position_ += translation; }

	// set the rotation in each axis to angle
	void Rotate(const float &xAngle, const float &yAngle, const float &zAngle);

	// increase/decrease each rotation angle by values
	void RotateBy(const float &xAngle, const float &yAngle, const float &zAngle);
	
	// This must be called in the Update of any object that has an instance of this class
	void UpdateTransformationMatrix();

	// return size of children_ vector 
	int GetChildCount() const { return children_.size(); }
	
	MyTransform* GetParent() const { return parent_; }
	void SetParent(MyTransform* trans) 
	{ 
		parent_ = trans; 
		trans->AddChild(this); 
	}

	MyTransform* GetChild(const int& n) const { return children_[n]; }
	void AddChild(MyTransform* trans) { children_.push_back(trans); }

	void SetActive(bool isActive) { is_active_ = isActive; }
	bool IsActive() { return is_active_; }

private:

	gef::Matrix44 local_transform_;
	gef::Matrix44 transform_world_;

	gef::Vector4 position_;
	float rotationX_;
	float rotationY_;
	float rotationZ_;
	gef::Vector4 scale_;	
	
	// this should only be set from the gameobject 
	bool is_active_;

	// Pointer to transform of parent
	MyTransform* parent_;

	// vector of pointers children_
	std::vector<MyTransform*> children_;
};

