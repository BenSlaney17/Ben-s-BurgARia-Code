#pragma once
#include <cstdlib>
namespace gef
{
	class Platform;
	class Renderer3D;
	class SpriteRenderer;
	class Font;
	class AudioManager;
	class InputManager;
}

namespace Ben
{
	class PlayerInput;
}

class GameState
{
public:
	GameState();
	virtual ~GameState()
	{
		//player_input_ = NULL;
	}

	virtual void OnEnter(gef::Platform& platform_, gef::AudioManager* audio_manager,  Ben::PlayerInput* player_input, gef::Renderer3D* renderer3D, gef::SpriteRenderer* sprite_renderer, gef::Font* font);
	// returns a pointer to the gamestate which should be loaded
	virtual GameState* Update(const float& frame_time){}
	virtual void Render(){}
	virtual void OnExit(){}

protected:
	gef::Platform* platform_;
	gef::AudioManager* audio_manager_;
	Ben::PlayerInput* player_input_;
	gef::Renderer3D* renderer_3d_;
	gef::SpriteRenderer* sprite_renderer_;
	gef::Font* font_;
};
