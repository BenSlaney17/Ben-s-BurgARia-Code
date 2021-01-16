#pragma once
#include <vector>
class GameObject;

class CollisionObject
{
public:
	CollisionObject();
	virtual~CollisionObject();

	void PrePhysicsUpdate();
	void PostPhysicsUpdate();
	void Colliding(GameObject* otherObj);

	bool IsColliding() const { return is_colliding_; }
	
protected:
	
	virtual void OnCollisionEnter(GameObject* otherObj) {}
	virtual void OnCollisionStay(GameObject* otherObj) {}
	virtual void OnCollisionExit(GameObject* otherObj) {}

private:
	bool AlreadyCollidingWithOtherObj(int i, GameObject * otherObj);

	bool is_colliding_;
	
	struct Collision
	{
		Collision(GameObject* obj)
		{
			object_ = obj;
			still_colliding_ = true;
		}
		GameObject* object_;
		bool still_colliding_;
	};

	std::vector<Collision> colliding_objects_;
};


