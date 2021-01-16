#pragma once

#include <graphics\texture.h>

namespace Ben
{
	class PlayerInput;
}

namespace gef
{
	class SpriteRenderer;
	class Sprite;
	class Font;
}
class CollisionEngine;
class GameObject;

class InteractionManager
{
public:
	InteractionManager(gef::Platform& platform, CollisionEngine* collision_engine, Ben::PlayerInput* player_input);
	~InteractionManager();

	void Update();
	void Render2DOverlay(gef::SpriteRenderer* sprite_renderer, gef::Font* font);

private:
	bool CanInteract() { return object_; }

	CollisionEngine * collision_engine_;
	Ben::PlayerInput* player_input_;
	GameObject* object_;

	gef::Texture* interaction_texture_;
	gef::Sprite* interaction_sprite_;
};

