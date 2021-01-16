#include "Button.h"
#include "graphics\sprite.h"
#include "graphics\sprite_renderer.h"
#include "graphics\texture.h"
#include "graphics\font.h"
#include "LoadSprite.h"
#include "PlayerInput.h"
#include "audio\audio_manager.h"


Button::Button( gef::Platform& platform, Ben::PlayerInput* player_input, const char * texture_filename, const char * text, gef::AudioManager* audio_manager)
	: player_input_(player_input),
	sprite_(NULL),
	texture_(NULL),
	text_(text),
	has_been_pressed_(false),
	font_size_(1.0f),
	audio_manager_(audio_manager),
	button_press_(-1)
{
	LoadSprite::InitSprite(platform, texture_, texture_filename, sprite_);
	// default width and height values
	sprite_->set_width(BUTTON_WIDTH_DEFAULT);
	sprite_->set_height(BUTTON_HEIGHT_DEFAULT);

//	button_press_ =  audio_manager_->LoadSample("ButtonSelect.wav", platform);
}

Button::Button(gef::Platform & platform, Ben::PlayerInput * player_input, const char * texture_filename, const char * text, gef::AudioManager * audio_manager, UInt32 button_press_sample_id)
	: player_input_(player_input),
	sprite_(NULL),
	texture_(NULL),
	text_(text),
	has_been_pressed_(false),
	font_size_(1.0f),
	audio_manager_(audio_manager),
	button_press_(button_press_sample_id)
{
	LoadSprite::InitSprite(platform, texture_, texture_filename, sprite_);
	// default width and height values
	sprite_->set_width(BUTTON_WIDTH_DEFAULT);
	sprite_->set_height(BUTTON_HEIGHT_DEFAULT);

	//	button_press_ =  audio_manager_->LoadSample("ButtonSelect.wav", platform);
}


Button::~Button()
{
	player_input_ = NULL;

	delete sprite_;
	sprite_ = NULL;

	delete texture_;
	texture_ = NULL;
}

void Button::Update(float frame_time)
{
	// check if I've been pressed
	gef::Vector2 touch_pos;
	if (player_input_->IsNewTouch(touch_pos))
	{
		if (Ben::TouchInput::IsInside(*sprite_, touch_pos))
		{
			has_been_pressed_ = true;
			// make sure button press sample ID is valid
			if (button_press_ >= 0)
			{
				audio_manager_->PlaySample(button_press_);
			}
			return;
		}
	}
	has_been_pressed_ = false;
	return;
}

void Button::Render(gef::SpriteRenderer * renderer, gef::Font * font)
{
	renderer->DrawSprite(*sprite_);
	font->RenderText(renderer, sprite_->position() - gef::Vector4(0, 10.0f,0), font_size_, 0xffffffff, gef::TJ_CENTRE, "%s", text_);
}

void Button::Scale(float s)
{
	sprite_->set_width(sprite_->width() * s);
	sprite_->set_height(sprite_->height() * s);
}

void Button::SetWidth(float w)
{
	sprite_->set_width(w);
}

void Button::SetHeight(float h)
{
	sprite_->set_height(h);
}
