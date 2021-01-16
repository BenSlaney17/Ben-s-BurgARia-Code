#include "CollisionObject.h"
#include "GameObject.h"
CollisionObject::CollisionObject()
{
	colliding_objects_ = std::vector<Collision>();
}

CollisionObject::~CollisionObject()
{
}

void CollisionObject::Colliding(GameObject * otherObj)
{
	for (int i = 0; i < colliding_objects_.size(); i++)
	{
		if (AlreadyCollidingWithOtherObj(i, otherObj))
		{
			OnCollisionStay(otherObj);
			colliding_objects_[i].still_colliding_ = true;
			return;
		}
	}

	// add this new object to the vector
	colliding_objects_.push_back(Collision(otherObj));
	OnCollisionEnter(otherObj);
}

bool CollisionObject::AlreadyCollidingWithOtherObj(int i, GameObject * otherObj)
{
	return colliding_objects_[i].object_ == otherObj;
}

void CollisionObject::PrePhysicsUpdate()
{
	// loop through all objects and set still_colliding to false
	// this should be called before any collision checks in CollisionEngine
	for (size_t i = 0; i < colliding_objects_.size(); i++)
	{
		colliding_objects_[i].still_colliding_ = false;
	}
}
                                                                                                                                                                                         
void CollisionObject::PostPhysicsUpdate()
{
	std::vector<int> vecPositionsToDelete = std::vector<int>();

	// if a colliding_object_ is no longer colliding then call OnCollisionExit and remove object from vector
	for (auto i = colliding_objects_.begin(); i != colliding_objects_.end(); ++i)
	{
		// if the currect object was not set to be colliding again then it is no longer colliding with this object
		int  index = std::distance(colliding_objects_.begin(), i);
		if (colliding_objects_[index].still_colliding_ == false)
		{
			OnCollisionExit(colliding_objects_[index].object_);
			colliding_objects_.erase(i);
			i--;
		}
	}
}
