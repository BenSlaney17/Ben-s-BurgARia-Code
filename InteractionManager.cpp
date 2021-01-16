#include "InteractionManager.h"
#include <graphics/font.h>
#include <graphics\sprite.h>
#include <graphics\sprite_renderer.h>

#include "CollisionEngine.h"
#include "PlayerInput.h"
#include "GameObject.h"
#include "GameSettings.h"
#include "load_texture.h"
InteractionManager::InteractionManager(gef::Platform& platform, CollisionEngine * collision_engine, Ben::PlayerInput* player_input)
	:collision_engine_(collision_engine), player_input_(player_input), object_(NULL), interaction_texture_(NULL), interaction_sprite_(NULL)
{
	interaction_texture_ = CreateTextureFromPNG("Target.png", platform);
	interaction_sprite_ = new gef::Sprite();
	interaction_sprite_->set_texture(interaction_texture_);
	interaction_sprite_->set_height(20.0f);
	interaction_sprite_->set_width(20.0f);
	interaction_sprite_->set_position(platform.width() / 2, platform.height() / 2, 0);
	
}

InteractionManager::~InteractionManager()
{
	collision_engine_ = NULL;
	player_input_ = NULL;
	object_ = NULL;
}

void InteractionManager::Update()
{
	object_ = collision_engine_->GetCurrentInteractableObject();
	
	if (object_ == NULL)
		return;

	if (player_input_->IsInputPressed("LeftBumper") || player_input_->IsInputPressed("RightBumper"))
		object_->Interaction();
}

void InteractionManager::Render2DOverlay(gef::SpriteRenderer * sprite_renderer,  gef::Font* font)
{
	// render dot in centre of screen 
	sprite_renderer->DrawSprite(*interaction_sprite_);

	if (!CanInteract()) return;

	if (font)
	{
		bool isRightHanded = GameSettings::Instance().GetAreControlsRightSided();
		if(isRightHanded)
			font->RenderText(sprite_renderer, gef::Vector4(50.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "L) %s", object_->GetInteractionText());
		else
		{
			font->RenderText(sprite_renderer, gef::Vector4(350, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "R) %s", object_->GetInteractionText());
		}
	}
}
