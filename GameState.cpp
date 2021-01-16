#include "GameState.h"

GameState::GameState()
	: platform_(NULL),
	player_input_(NULL),
	renderer_3d_(NULL),
	sprite_renderer_(NULL),
	font_(NULL)
{
}

void GameState::OnEnter(gef::Platform& platform, gef::AudioManager* audio_manager, Ben::PlayerInput * player_input, gef::Renderer3D * renderer3D, gef::SpriteRenderer * sprite_renderer, gef::Font* font)
{
	platform_ = &platform;
	audio_manager_ = audio_manager;
	player_input_ = player_input;
	renderer_3d_ = renderer3D; 
	sprite_renderer_ = sprite_renderer;
	font_ = font ;
}
